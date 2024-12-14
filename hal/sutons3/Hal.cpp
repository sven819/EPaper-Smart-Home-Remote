#include <lvgl.h>
#include "HAL.hpp"
#include <WiFi.h>

static const char *TAG = "gui";

static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 800;
static uint16_t chunkCnt = 0;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][screenWidth * 10];

LGFX gfx;
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60       /* Time ESP32 will go to sleep (in seconds) */

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
// Clear Parameter nur relevant für EPD
void HAL::init(bool dummy)
{
    lv_init();

    gfx.begin();

    gfx.setBrightness(255);
    gfx.setRotation(1);

    ::lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = this->tft_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = HAL::touchscreen_read;
    lv_indev_drv_register(&indev_drv);
}

void HAL::touchscreen_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if (gfx.getTouch(&touchX, &touchY))
    {
        // lv_disp_enable_invalidation(NULL, true);
        lv_disp_enable_invalidation(NULL, true);
        ESP_LOGW("HAL", "ABCBC");

        data->state = LV_INDEV_STATE_PR;

        data->point.x = touchX;
        data->point.y = touchY;
        // Notify bei Touch event
        TaskHandle_t taskHandle = xTaskGetHandle("pwrMngmntTask");
        if (taskHandle != nullptr)
            xTaskNotify(taskHandle, 2, eSetValueWithOverwrite);
    }
}

void HAL::tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{

    if (chunkCnt == 0)
    {
        gfx.startWrite();
        chunkCnt = 1;
    }

    gfx.pushImage(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::rgb565_t *)&color_p->full);
    if (lv_disp_flush_is_last(disp))
    {
        gfx.endWrite();
        chunkCnt = 0;
    }
    lv_disp_flush_ready(disp);
}
