#include "../icons/light.h"
#include "Sensor.hpp"

static const char *TAG = "SensorButton";

const lv_img_dsc_t *SensorButton::getStateIcon()
{
    return nullptr;
}
void SensorButton::label_event(lv_event_t *e)
{
    // lv_disp_enable_invalidation(NULL, false);
    this->state = this->dev->getState();
    Sensor *d = (Sensor *)this->dev;
    this->value = std::stoi((d->value.c_str()));
    this->unit = d->unit;
    lv_label_set_text_fmt(this->valueLabel, "%d%s", this->value, this->unit.c_str());
}
void SensorButton::drawAdditionalElements(lv_obj_t *cui_TileButton)
{
    this->valueLabel = lv_label_create(cui_TileButton);
    lv_obj_set_width(this->valueLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(this->valueLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(this->valueLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_y(this->valueLabel, 50);
    lv_obj_set_style_text_font(this->valueLabel, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(this->valueLabel, "%d%s", this->value, this->unit.c_str());
}