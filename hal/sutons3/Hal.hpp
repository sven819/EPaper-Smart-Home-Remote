#pragma once
#include "IHal.hpp"
#include "driver.h"

class HAL : public IHalInterface
{
public:
    void init(bool dummy) override;
    void startSleep() override;
    tm getTime() override;
    bool networkConnected() override;

protected:
    static void touchscreen_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
    static void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
};