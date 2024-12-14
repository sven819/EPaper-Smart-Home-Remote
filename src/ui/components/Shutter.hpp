#pragma once

#include <lvgl.h>
#include "ActionButton.hpp"
#include "../icons/climate.h"

class ShutterButton : public ActionButton
{
public:
  ShutterButton(Shutter *dev) : ActionButton(dev), dev(dev)
  {
    this->value = dev->height;
    this->unit = "%";
  };

protected:
  Shutter *dev;

private:
  const lv_img_dsc_t *getStateIcon() override;
  void label_event(lv_event_t *e) override;
  void click_event_cb(lv_event_t *e);
};
