#pragma once

#include <lvgl.h>
#include "../../model/settings/views.hpp"
#include "../../IoTDevs.hpp"

typedef struct
{
    view_t *viewPtr;
    DeviceStore *store;
    PubSub<DeviceCommand> *commandPubSub;
} tile_fragment_args_t;

typedef struct tile_fragment_t
{
    lv_fragment_t base;
    lv_obj_t *label;
    view_t *viewData;
    DeviceStore *store;
    PubSub<DeviceCommand> *commandPubSub;

} tile_fragment_t;

static void tile_fragment_ctor(lv_fragment_t *self, void *args);
static lv_obj_t *tile_fragment_create_obj(lv_fragment_t *self, lv_obj_t *parent);

extern lv_fragment_class_t tile_cls;