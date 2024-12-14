#pragma once
#include <lvgl.h>
#include "ActionButton.hpp"

#include "../icons/climate.h"

class ClimateButton : public ActionButton
{
public:
  ClimateButton(IoTDevice *dev) : ActionButton(dev)
  {
    // esp_log_write(ESP_LOG_DEBUG, TAG, "Light Button Construct");
    this->unit = "°C";
    this->value = 21;
  };

private:
  const lv_img_dsc_t *getStateIcon() override;
};
