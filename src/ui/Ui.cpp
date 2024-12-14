#include <lvgl.h>
#include "views/TileView.hpp"
#include "views/Menu.hpp"
#include "Ui.hpp"
#include "../IotDevs.hpp"
#include "events/events.hpp"
#include "../PubSub.hpp"
#include "IHal.hpp"
#include "Statusbar.hpp"

static const char *TAG = "UI";

void UI::setStateChangeCallback(StateChangeCallback newCallback)
{
    callback = newCallback;
}

void UI::navEvent()
{
    ESP_LOGV(TAG, "Event");
}

void UI::namedView(std::string viewname)
{
    ESP_LOGI(TAG, "Route CLicked:%s\n", viewname.c_str());
    lv_fragment_manager_pop(manager);

    auto it = std::find_if(views.begin(), views.end(),
                           [viewname](const view_t &obj)
                           {
                               return obj.name == viewname;
                           });

    if (it != views.end())
    {
        if (callback)
        {
            callback(viewname);
        }
        ESP_LOGV(TAG, "Route gefunden");

        tile_fragment_args_t a;
        a.store = deviceStore;
        a.viewPtr = &*it;
        a.commandPubSub = this->commandPubSub;
        // a.viewPtr = &views.at(0);
        lv_fragment_t *fragm = lv_fragment_create(&tile_cls, &a);

        pushFragment(fragm);
    }
    else
    {
        ESP_LOGE(TAG, "Route nicht gefunden : %s", viewname.c_str());
    }
}

void UI::handleMenuClick()
{
    auto lambda = [this](std::string arg)
    {
        this->namedView(arg);
    };

    menu_fragment_args_t a;
    a.views = &views;
    a.callback = lambda;

    lv_fragment_t *fragm = lv_fragment_create(&menu_cls, &a);
    pushFragment(fragm);
}

void UI::draw(std::string viewname, bool blocking)
{
    ESP_LOGV(TAG, "Enter UI Draw");

    root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(root, LV_LAYOUT_GRID);

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_grid_dsc_array(root, col_dsc, row_dsc);
    container = lv_obj_create(root);
    lv_obj_remove_style_all(container);
    lv_obj_set_grid_cell(container, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_t *float_btn_min = lv_btn_create(root);
    lv_obj_set_size(float_btn_min, 75, 75);
    lv_obj_add_flag(float_btn_min, LV_OBJ_FLAG_FLOATING);
    lv_obj_align(float_btn_min, LV_ALIGN_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_radius(float_btn_min, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_img_src(float_btn_min, LV_SYMBOL_LIST, 0);
    lv_obj_set_style_text_font(float_btn_min, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);

    manager = lv_fragment_manager_create(NULL);

    lv_obj_add_event_cb(
        float_btn_min, [](lv_event_t *e)
        { UI *handler = (UI *)lv_event_get_user_data(e);
        handler->handleMenuClick(); },
        LV_EVENT_CLICKED, this);

    this->statusbar = new Statusbar(this->hal, root);

    if (!viewname.empty())
        this->namedView(viewname);
    else
        handleMenuClick();

    auto lvglTaskLambda = [](void *pvParameters)
    {
        auto *instance = static_cast<UI *>(pvParameters);

        for (;;)
        {
            instance->handle();
        }
    };

    if (blocking)
    {
        while (!this->hal->displayReady)
        {
            lv_task_handler();
            vTaskDelay(50);
        }
    }
    else
    {
        BaseType_t uiTask = xTaskCreatePinnedToCore(lvglTaskLambda, "UITask", 8000, this, 4, NULL, 1);
    }
}

void UI::pushFragment(lv_fragment_t *fragment)
{
    lv_fragment_manager_push(manager, fragment, &container);
}

void UI::handle()
{
    DeviceStatus status;

    if (this->statusPubSub->receive(this->clientID, status, pdMS_TO_TICKS(100)))
    {
        lv_msg_send(UIEvents::DEV_VALUE_CHANGE, status.device_id);
    }
    lv_task_handler();
}

void UI::init(std::vector<view_t> views, DeviceStore *store, PubSub<DeviceStatus> *_statusPubSub, PubSub<DeviceCommand> *_commandPubSub, IHalInterface *hal)
{
    this->statusPubSub = _statusPubSub;
    this->commandPubSub = _commandPubSub;
    this->clientID = statusPubSub->subscribe();
    this->hal = hal;
    deviceStore = store;
    this->views = views;
}