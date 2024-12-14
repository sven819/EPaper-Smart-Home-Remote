#pragma once

#include <lvgl.h>

#include "../icons/climate.h"
#include "../../IoTDevs.hpp"

using NotifyCallback = std::function<void(DeviceCommand)>;

class TileButton
{

public:
    std::string id;
    const char *description;
    bool state;
    lv_obj_t *create_ui_element(lv_obj_t *comp_parent);
    TileButton(IoTDevice *dev);
    ~TileButton();
    void setCallback(NotifyCallback newCallback);
    void notify(DeviceCommand cmd);

private:
    virtual const lv_img_dsc_t *getStateIcon();
    virtual void label_event(lv_event_t *e);
    lv_obj_t *drawBaseElement(lv_obj_t *comp_parent);

    virtual void drawAdditionalElements(lv_obj_t *cui_TileButton);

    static void destruct(lv_event_t *e);
    virtual void toggle_state(lv_event_t *e);
    static void update(bool state);

    NotifyCallback callback;
    virtual void longPressEvent() { return; };

protected:
    IoTDevice *dev;
    lv_obj_t *icon;
    lv_obj_t *base;
};