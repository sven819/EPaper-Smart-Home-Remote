#include <lvgl.h>
#include "Tileview.hpp"
#include "../components/Light.hpp"
#include "../components/Climate.hpp"
#include "../components/Sensor.hpp"
#include "../components/Shutter.hpp"
#include "../components/ColorLight.hpp"
#include "../components/DimmLight.hpp"
#include "../components/Media.hpp"

#include "../../model/settings/views.hpp"
#include "../../PubSub.hpp"

static const char *TAG = "TileView";

static void tile_fragment_ctor(lv_fragment_t *self, void *args)
{
    LV_UNUSED(args);
    tile_fragment_args_t *fragmentArgs = (tile_fragment_args_t *)args;
    ((tile_fragment_t *)self)->viewData = fragmentArgs->viewPtr;
    ((tile_fragment_t *)self)->store = fragmentArgs->store;
    ((tile_fragment_t *)self)->commandPubSub = fragmentArgs->commandPubSub;
}

static lv_obj_t *tile_fragment_create_obj(lv_fragment_t *self, lv_obj_t *parent)
{
    tile_fragment_t *fragment = (tile_fragment_t *)self;
    lv_obj_t *content = lv_obj_create(parent);

    lv_obj_remove_style_all(content);

    lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(content, LV_OPA_0, 0);

    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_height(content, lv_pct(90));
    lv_obj_set_align(content, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_pad_left(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(content, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(content, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(content, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    DeviceStore *store = fragment->store;
    PubSub<DeviceCommand> *pubSub = fragment->commandPubSub;

    for (std::string dev : fragment->viewData->devices)
    {
        IoTDevice *dev1 = (IoTDevice *)store->getDevice(dev);
        if (dev1 != nullptr)
        {
            TileButton *btn;
            if (ColorLight *d = dynamic_cast<ColorLight *>(dev1); d != nullptr)
            {
                btn = new ColorLightButton(d);
            }
            else if (DimmLight *d = dynamic_cast<DimmLight *>(dev1); d != nullptr)
            {
                btn = new DimmLightButton(d);
            }
            else if (Shutter *d = dynamic_cast<Shutter *>(dev1); d != nullptr)
            {
                btn = new ShutterButton(d);
            }
            else if (Media *d = dynamic_cast<Media *>(dev1); d != nullptr)
            {
                btn = new MediaButton(dev1);
            }
            else if (Light *d = dynamic_cast<Light *>(dev1); d != nullptr)
            {
                btn = new LightButton(dev1);
            }
            else if (Climate *d = dynamic_cast<Climate *>(dev1); d != nullptr)
            {
                btn = new ClimateButton(dev1);
            }
            else if (Sensor *d = dynamic_cast<Sensor *>(dev1); d != nullptr)
            {
                btn = new SensorButton(d);
            }
            btn->create_ui_element(content);
            btn->setCallback([pubSub](DeviceCommand cmd)
                             { pubSub->publish(cmd); });
        }
    }

    return content;
};

lv_fragment_class_t tile_cls = {
    .constructor_cb = tile_fragment_ctor,
    .create_obj_cb = tile_fragment_create_obj,
    .instance_size = sizeof(tile_fragment_t)};
