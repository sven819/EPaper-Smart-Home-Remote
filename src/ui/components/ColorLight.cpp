#include "../icons/light.h"
#include "ColorLight.hpp"

static const char *TAG = "ColorLightButton";

const lv_img_dsc_t *ColorLightButton::getStateIcon()
{
    if (this->state)
        return &light_on;
    else
        return &light_off;
}

// todo klasse auslagen
void ColorLightButton::colorElement(lv_obj_t *cont, const char *colorName, const RGB *color, lv_event_cb_t event)
{
    lv_obj_t *abc = lv_obj_create(cont);
    lv_obj_set_width(abc, 200);
    lv_obj_set_height(abc, 200);
    lv_obj_set_x(abc, -182);
    lv_obj_set_y(abc, -61);
    lv_obj_set_align(abc, LV_ALIGN_CENTER);
    lv_obj_clear_flag(abc, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(abc, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(abc, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(abc, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(abc, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(abc, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(abc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(abc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(abc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(abc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_user_data(abc, (void *)color);

    lv_obj_t *colorNameLabel = lv_label_create(abc);
    lv_obj_set_width(colorNameLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(colorNameLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(colorNameLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(colorNameLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(colorNameLabel, colorName);

    lv_obj_add_event_cb(abc, event, LV_EVENT_CLICKED, (void *)this);
}
void ColorLightButton::setColor(RGB *color)
{
    // todo
    DeviceCommand cmd;
    strcpy(cmd.device_id, this->dev->getId().c_str());
    cmd.action = static_cast<int>(ColorLight::supportedActions::setColor);
    this->dev->color = *color;
    this->notify(cmd);
}

void ColorLightButton::longPressEvent()
{
    win = lv_win_create(lv_scr_act(), 40);
    lv_obj_t *cont = lv_win_get_content(win);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *float_btn_min = lv_btn_create(cont);
    lv_obj_set_size(float_btn_min, 75, 75);
    lv_obj_add_flag(float_btn_min, LV_OBJ_FLAG_FLOATING);
    lv_obj_align(float_btn_min, LV_ALIGN_BOTTOM_LEFT, 25, -25);
    lv_obj_set_style_radius(float_btn_min, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_img_src(float_btn_min, LV_SYMBOL_CLOSE, 0);
    lv_obj_set_style_text_font(float_btn_min, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(
        float_btn_min, [](lv_event_t *e)
        { ColorLightButton *_this = (ColorLightButton *)lv_event_get_user_data(e);
        lv_obj_del(_this->win); },
        LV_EVENT_CLICKED, this);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(cont, LV_OPA_100, 0);
    lv_obj_set_width(cont, lv_pct(100));
    lv_obj_set_height(cont, lv_pct(90));
    lv_obj_set_align(cont, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_pad_left(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cont, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    auto colorLamba = [](lv_event_t *e)
    {
        RGB *color = (RGB *)lv_obj_get_user_data(lv_event_get_target(e));
        ColorLightButton *_this = (ColorLightButton *)lv_event_get_user_data(e);
        _this->setColor(color);
    };
    for (const auto &item : predefinedColors)
    {
        colorElement(cont, item.name.c_str(), &item.color, colorLamba);
    }
}
