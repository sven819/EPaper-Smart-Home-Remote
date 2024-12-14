
#include <vector>
#include <lvgl.h>
#include "Menu.hpp"
#include "../components/Light.hpp"
#include "../../model/settings/views.hpp"


static const char *TAG = "MenuView";

static void menu_fragment_ctor(lv_fragment_t *self, void *args)
{
    LV_UNUSED(args);
    menu_fragment_args_t *fragmentArgs = (menu_fragment_args_t *)args;
    ((menu_fragment_t *)self)->views = fragmentArgs->views;
    ((menu_fragment_t *)self)->callback = fragmentArgs->callback;
}

static lv_obj_t *menu_fragment_create_obj(lv_fragment_t *self, lv_obj_t *parent)
{
    menu_fragment_t *fragment = (menu_fragment_t *)self;
    lv_obj_t *content = lv_obj_create(parent);

    lv_obj_remove_style_all(content);

    lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_height(content, lv_pct(90));
    lv_obj_set_align(content, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_pad_left(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(content, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(content, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(content, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    for (view_t view : *fragment->views)
    {
        lv_obj_t *main_label = lv_label_create(content);
        lv_label_set_text(main_label, view.name.c_str());

        lv_obj_set_style_text_font(main_label, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_width(main_label, lv_pct(100));
        lv_obj_set_style_text_align(main_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_flag(main_label, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_add_event_cb(
            main_label, [](lv_event_t *e)
            {
                menu_fragment_t *fragment = (menu_fragment_t *)lv_event_get_user_data(e);
                fragment->callback(lv_label_get_text(e->target)); },
            LV_EVENT_CLICKED, fragment);
    }
    return content;
};

lv_fragment_class_t menu_cls = {
    .constructor_cb = menu_fragment_ctor,
    .create_obj_cb = menu_fragment_create_obj,
    .instance_size = sizeof(menu_fragment_t)};
