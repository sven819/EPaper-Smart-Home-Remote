#pragma once

#include "../model/settings/views.hpp"
#include <lvgl.h>
#include "../IoTDevs.hpp"
#include "../PubSub.hpp"
#include "IHal.hpp"
#include "Statusbar.hpp"

using StateChangeCallback = std::function<void(const std::string &)>;

class UI
{
public:
    void handleMenuClick();
    void draw(std::string viewname, bool blocking = false);
    void handle();
    void navEvent();
    void pushFragment(lv_fragment_t *fragment);
    void namedView(std::string viewname);
    void init(std::vector<view_t> views, DeviceStore *store, PubSub<DeviceStatus> *_statusPubSub, PubSub<DeviceCommand> *_commandPubSub, IHalInterface *hal);
    void setStateChangeCallback(StateChangeCallback newCallback);
    ~UI()
    {
    }

private:
    lv_obj_t *root;
    lv_obj_t *container;
    lv_obj_t *label;
    lv_fragment_manager_t *manager;
    DeviceStore *deviceStore;
    std::vector<view_t> views;
    StateChangeCallback callback;
    int clientID;
    PubSub<DeviceStatus> *statusPubSub;
    PubSub<DeviceCommand> *commandPubSub;
    IHalInterface *hal;
    Statusbar *statusbar;
};
