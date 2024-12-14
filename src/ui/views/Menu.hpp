#pragma once

#include <lvgl.h>
#include "model/settings/views.hpp"
#include "IoTDevs.hpp"

typedef struct
{
    std::vector<view_t> *views;
    std::function<void(std::string)> callback;
} menu_fragment_args_t;

typedef struct menu_fragment_t
{
    lv_fragment_t base;
    lv_obj_t *label;
    std::vector<view_t> *views;
    std::function<void(std::string)> callback;

} menu_fragment_t;

static void menu_fragment_ctor(lv_fragment_t *self, void *args);
static lv_obj_t *menu_fragment_create_obj(lv_fragment_t *self, lv_obj_t *parent);

extern lv_fragment_class_t menu_cls;
