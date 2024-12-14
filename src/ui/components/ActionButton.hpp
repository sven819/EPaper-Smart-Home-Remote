#pragma once
#include <lvgl.h>
#include "TileButton.hpp"

#include "../icons/climate.h"

class ActionButton : public TileButton
{
public:
  ActionButton(IoTDevice *dev) : TileButton(dev){};
  ActionButton(IoTDevice *dev, int _val, std::string _unit) : TileButton(dev)
  {
    this->value = _val;
    this->unit = _unit;
  };

private:
  const lv_img_dsc_t *getStateIcon() override;
  void drawAdditionalElements(lv_obj_t *cui_TileButton) override;
  virtual void click_event_cb(lv_event_t *e);

protected:
  int value;
  std::string unit;
  lv_obj_t *label;
  lv_obj_t *plus;
  lv_obj_t *minus;
};
