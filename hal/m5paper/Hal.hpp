#pragma once

#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#define screenWidth 540
#define screenHeight 960
#define BAT_ADC_CHANNEL ADC1_GPIO35_CHANNEL
#define BASE_VOLATAGE 3600
#define SCALE_INV 2
#define ADC_FILTER_SAMPLE 8
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 60
#define LGFX_USE_V1
#define LGFX_M5PAPER

#include <SPI.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <RtcPCF8563.h>
#include "IHal.hpp"

class HAL : public IHalInterface
{
public:
    void init(bool synchronStart) override;
    void startSleep() override;
    tm getTime() override;
    uint16_t batteryState() override;
    bool networkConnected() override;
    bool factoryResetEnter() override;

protected:
    void touchscreen_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
    void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

private:
    void BatteryADCBegin();
    esp_adc_cal_characteristics_t *adc_chars;
    uint32_t getBatteryVoltage();
    lv_disp_draw_buf_t draw_buf;
    lv_color_t buf[screenWidth * 10];
    uint16_t chunkCnt = 0;
    uint32_t pxl = 0;
    SemaphoreHandle_t displaySemaphore;
    bool adcInit = false;
};