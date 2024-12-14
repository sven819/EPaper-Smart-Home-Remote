#include "../icons/climate.h"
#include "Climate.hpp"

static const char *TAG = "ActionButton";

const lv_img_dsc_t *ActionButton::getStateIcon()
{
    if (this->state)
        return &climate_on;
    else
        return &climate_off;
}

void ActionButton::click_event_cb(lv_event_t *e)
{
    lv_obj_t *clickTarget = lv_event_get_current_target(e);
    lv_obj_t **comp_TileButton = (lv_obj_t **)lv_event_get_user_data(e);

    if (clickTarget == this->minus)
    {
        this->value--;
    }
    else
        this->value++;

    lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());
}
void ActionButton::drawAdditionalElements(lv_obj_t *cui_TileButton)
{
    // Aktions Button
    lv_obj_t *cui_Panel4;
    cui_Panel4 = lv_obj_create(cui_TileButton);
    lv_obj_set_width(cui_Panel4, 200);
    lv_obj_set_height(cui_Panel4, 50);
    lv_obj_set_x(cui_Panel4, 0);
    lv_obj_set_y(cui_Panel4, 71);
    lv_obj_set_align(cui_Panel4, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(cui_Panel4, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cui_Panel4, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(cui_Panel4, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(cui_Panel4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Panel4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Panel4, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(cui_Panel4, LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Minus Aktion Button
    this->minus = lv_obj_create(cui_Panel4);
    lv_obj_set_width(this->minus, 100);
    lv_obj_set_height(this->minus, 50);
    lv_obj_set_x(this->minus, -70);
    lv_obj_set_y(this->minus, -192);
    lv_obj_set_align(this->minus, LV_ALIGN_CENTER);
    lv_obj_clear_flag(this->minus, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(this->minus, lv_color_hex(0x170606), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(this->minus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(this->minus, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(this->minus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(this->minus, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(this->minus, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Label2;
    cui_Label2 = lv_label_create(this->minus);
    lv_obj_set_width(cui_Label2, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(cui_Label2, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(cui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label2, "-");
    lv_obj_add_flag(cui_Label2, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_text_font(cui_Label2, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Ende Minus Aktion‚

    // Plus Aktion

    this->plus = lv_obj_create(cui_Panel4);
    lv_obj_set_width(this->plus, 100);
    lv_obj_set_height(this->plus, 51);
    lv_obj_set_x(this->plus, 0);
    lv_obj_set_y(this->plus, 59);
    lv_obj_clear_flag(this->plus, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(this->plus, lv_color_hex(0x170606), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(this->plus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(this->plus, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(this->plus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(this->plus, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(this->plus, LV_BORDER_SIDE_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Label3;
    cui_Label3 = lv_label_create(this->plus);
    lv_obj_set_width(cui_Label3, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(cui_Label3, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(cui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label3, "+");
    lv_obj_add_flag(cui_Label3, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_text_font(cui_Label3, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
    // Ende Plus Aktion

    // Aditional Value
    this->label = lv_label_create(cui_TileButton);
    lv_obj_set_width(this->label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(this->label, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(this->label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(this->label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());

    auto click_event_lambda = [](lv_event_t *e)
    {
        ActionButton *idd = (ActionButton *)lv_event_get_user_data(e);

        idd->click_event_cb(e);
    };

    lv_obj_add_event_cb(this->minus, click_event_lambda, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(this->plus, click_event_lambda, LV_EVENT_CLICKED, this);
}
