#pragma once

#include <lvgl.h>
#include "DimmLight.hpp"
#include "../icons/climate.h"

struct color_t
{
  std::string name;
  RGB color;
  color_t(const std::string &name, const RGB &color) : name(name), color(color) {}
};

class ColorLightButton : public DimmLightButton
{
public:
  ColorLightButton(ColorLight *dev) : DimmLightButton(dev), dev(dev){};

protected:
  ColorLight *dev;

private:
  const lv_img_dsc_t *getStateIcon() override;
  void longPressEvent() override;
  lv_obj_t *win;
  void setColor(RGB *color);
  void colorElement(lv_obj_t *cont, const char *colorName, const RGB *color, lv_event_cb_t event);
  std::vector<color_t> predefinedColors PROGMEM = {
      {"Weiss", RGB(255, 242, 224)},
      {"Kaltweiss", RGB(255, 255, 255)},
      {"Lavendel", RGB(230, 230, 250)},
      {"Pfirsich", RGB(255, 218, 185)},
      {"Aquamarin", RGB(127, 255, 212)},
      {"Moosgruen", RGB(0, 128, 0)}};
};
