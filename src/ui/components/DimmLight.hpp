#pragma once

#include <lvgl.h>
#include "ActionButton.hpp"
#include "../icons/climate.h"

class DimmLightButton : public ActionButton
{
public:
  DimmLightButton(DimmLight *dev) : ActionButton(dev), dev(dev)
  {
    this->value = dev->brightness;
    this->unit = "%";
  };

protected:
  DimmLight *dev;

private:
  void click_event_cb(lv_event_t *e) override;
  const lv_img_dsc_t *getStateIcon() override;
  void label_event(lv_event_t *e) override;
};
