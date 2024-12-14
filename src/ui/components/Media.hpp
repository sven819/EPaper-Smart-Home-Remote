#pragma once

#include <lvgl.h>
#include "TileButton.hpp"
#include "../icons/climate.h"

class MediaButton : public TileButton
{
public:
  MediaButton(IoTDevice *dev) : TileButton(dev){
                                    // esp_log_write(ESP_LOG_DEBUG, TAG, "Light Button Construct");
                                };

private:
  const lv_img_dsc_t *getStateIcon() override;
  void drawAdditionalElements(lv_obj_t *cui_TileButton) override;
};
