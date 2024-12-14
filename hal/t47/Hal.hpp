#pragma once
#include "IHal.hpp"
#include <touch.h>

class HAL : public IHalInterface
{
public:
    void init(bool dummy) override;
    void startSleep() override;
    tm getTime() override;
    bool networkConnected() override;

protected:
    void touchscreen_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
    void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    static const uint16_t screenWidth = 960;
    static const uint16_t screenHeight = 540;
    lv_disp_draw_buf_t draw_buf;
    lv_color_t buf[screenWidth * 10];
    uint8_t *fb_buffer;
    lv_area_t cmpl_area;
    int chunkCnt;
};