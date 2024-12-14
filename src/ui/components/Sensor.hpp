#pragma once

#include <lvgl.h>
#include "TileButton.hpp"
#include "../icons/climate.h"
#include <string>

class SensorButton : public TileButton
{
public:
  SensorButton(Sensor *dev) : TileButton(dev)
  {
    try
    {
      this->value = std::stoi(dev->value);
    }
    catch (const std::invalid_argument &e)
    {
      this->value = 0;
    }
    catch (const std::out_of_range &e)
    {
      this->value = 0;
    }
    this->unit = dev->unit;
  };

private:
  lv_obj_t *valueLabel;
  std::string unit;
  const lv_img_dsc_t *getStateIcon() override;
  void label_event(lv_event_t *e) override;
  void drawAdditionalElements(lv_obj_t *cui_TileButton) override;
  int value;
};
