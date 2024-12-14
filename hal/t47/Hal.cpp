#include <esp_task_wdt.h>
#include <lvgl.h>
#include <Wire.h>
#include <touch.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Hal.hpp"
#include "epd_driver.h"
#include "pins.h"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60       /* Time ESP32 will go to sleep (in seconds) */
TouchClass touch;

void HAL::startSleep()
{
  // M5.disableEXTPower();
  // gpio_hold_en((gpio_num_t)2);
  // gpio_deep_sleep_hold_en();
  // https: // cat-in-136.github.io/2022/05/note-m5paper-power-supply-management.html
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW); // TOUCH_INT

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void delayedTask(void *param)
{
  vTaskDelay(pdMS_TO_TICKS(10000)); // 5 Sekunden Verzögerung
  // todo: task nicht erstellen wenn er von diesem ausgelöst wird
  // lv_obj_invalidate(lv_scr_act());
  vTaskDelete(NULL); // Löscht den aktuellen Task
}

void HAL::init(bool dummy)
{
  this->displayReady = false;

  lv_init();

  pinMode(TOUCH_INT, INPUT_PULLUP);

  Wire.begin(TOUCH_SDA, TOUCH_SCL);

  if (!touch.begin())
  {
    Serial.println("start touchscreen failed");
    while (1)
      ;
  }
  Serial.println("Started Touchscreen poll...");
  epd_init();

  ::lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  static lv_disp_drv_t disp_drv;
  ::lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.user_data = this;

  disp_drv.flush_cb = [](struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
  {
    HAL *hal = (HAL *)disp_drv->user_data;
    hal->tft_flush(disp_drv, area, color_p);
  };
  disp_drv.draw_buf = &draw_buf;
  disp_drv.sw_rotate = 1;
  disp_drv.rotated = LV_DISP_ROT_90;

  ::lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);          /*Initialize the input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER; /*The touchpad is a pointer-like device*/
  indev_drv.user_data = this;

  indev_drv.read_cb = [](struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
  {
    HAL *hal = (HAL *)indev_drv->user_data;
    hal->touchscreen_read(indev_drv, data);
  };

  lv_indev_drv_register(&indev_drv); /*Finally register the driver*/

  fb_buffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!fb_buffer)
  {
    Serial.println("alloc memory failed !!!");
    while (1)
      ;
  }
  memset(fb_buffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  epd_poweron();
  epd_clear();
  //  epd_poweroff();
}

void HAL::touchscreen_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  data->state = LV_INDEV_STATE_REL;

  if (touch.scanPoint())
  {
    touch.getPoint(touchX, touchY, 0);
    data->state = LV_INDEV_STATE_PR;
    // wert invertieren durch drehung
    data->point.y = 525 - touchY;
    data->point.x = touchX;
  }
}

void HAL::tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  if (chunkCnt == 0)
  {
    lv_area_copy(&cmpl_area, area);
    chunkCnt = 1;
  }
  else
  {
    if (area->x1 < cmpl_area.x1)
    {
      cmpl_area.x1 = area->x1;
    }
    if (area->x2 > cmpl_area.x2)
    {
      cmpl_area.x2 = area->x2;
    }
    if (area->y1 < cmpl_area.y1)
    {
      cmpl_area.y1 = area->y1;
    }
    if (area->y2 > cmpl_area.y2)
    {
      cmpl_area.y2 = area->y2;
    }
  }
  int32_t x, y;
  for (y = area->y1; y <= area->y2; y++)
  {
    for (x = area->x1; x <= area->x2; x++)
    {
      // Serial.println("DRA");
      uint8_t col = lv_color_brightness(*color_p);
      epd_draw_pixel(x, y, col, fb_buffer);
      color_p++;
    }
  }

  if (lv_disp_flush_is_last(disp))
  {
    const uint32_t w = cmpl_area.x2 - cmpl_area.x1 + 1;
    const uint32_t h = cmpl_area.y2 - cmpl_area.y1 + 1;

    Rect_t area_draw = {
        .x = cmpl_area.x1,
        .y = cmpl_area.y1,
        .width = w,
        .height = h};
    epd_clear_area_cycles(epd_full_screen(), 1, 10);
    epd_draw_grayscale_image(epd_full_screen(), fb_buffer);

    //   epd_poweron();
    // epd_clear_area_cycles(area_draw, 1, 50);
    //   epd_clear_area_cycles(area_draw, 1, 10);
    //   epd_draw_grayscale_image(epd_full_screen(), fb_buffer);
    //   epd_poweroff();
    chunkCnt = 0;
    //   TaskHandle_t taskHandle = xTaskGetHandle("DelayedTask");
    //   if (taskHandle != NULL)
    //   {
    //     vTaskDelete(taskHandle);
    //   }
    //   xTaskCreate(delayedTask, "DelayedTask", 2048, NULL, 5, NULL);
    this->displayReady = true;
  }

  lv_disp_flush_ready(disp);
}

tm HAL::getTime()
{
  tm time;
  time.tm_min = 43;
  time.tm_hour = 13;
  return time;
}
bool HAL::networkConnected()
{
  return WiFi.status() == WL_CONNECTED;
};