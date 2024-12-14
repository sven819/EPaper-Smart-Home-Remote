#include <lvgl.h>
#include "TileButton.hpp"
#include "../events/Events.hpp"
#include <stdlib.h>
#include "../../IoTDevs.hpp"

static const char *TAG = "UI";

void TileButton::label_event(lv_event_t *e)
{

    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_current_target(e);
    lv_msg_t *m = lv_event_get_msg(e);
    char *id = (char *)lv_msg_get_payload(m);

    if (strcmp(this->id.c_str(), id) == 0)
    {

        lv_obj_t **comp_TileButton = (lv_obj_t **)lv_event_get_user_data(e);
        if (event_code == LV_EVENT_MSG_RECEIVED)
        {
            this->state = this->dev->getState();
            lv_img_set_src(this->icon, this->getStateIcon());
        }
    }
}

void TileButton::setCallback(NotifyCallback newCallback)
{
    callback = newCallback;
}

void TileButton::notify(DeviceCommand cmd)
{
    if (callback)
    {
        callback(cmd);
    }
}

TileButton::TileButton(IoTDevice *dev)
{
    this->id = dev->getId();
    this->state = dev->getState();
    this->dev = dev;
    ESP_LOGV(TAG, "CONSTRUCTOR STATE %d BTN: %s ", this->state, this->id.c_str());
};

TileButton::~TileButton()
{

    ESP_LOGE(TAG, "TileButtonDestruct");
};

const lv_img_dsc_t *TileButton::getStateIcon()
{
    if (this->state)
        return &climate_on;
    else
        return &climate_off;
}

void TileButton::destruct(lv_event_t *e)
{
    TileButton *idd = (TileButton *)lv_event_get_user_data(e);
    delete idd;
}

void TileButton::toggle_state(lv_event_t *e)
{
    ESP_LOGV(TAG, "Action Button Toggle");
    this->state = !this->state;
    lv_img_set_src(this->icon, this->getStateIcon());

    DeviceCommand cmd;
    strcpy(cmd.device_id, this->dev->getId().c_str());
    cmd.action = static_cast<int>(IoTDevice::supportedActions::setState);

    this->notify(cmd);
}

lv_obj_t *TileButton::drawBaseElement(lv_obj_t *comp_parent)
{

    this->base = lv_obj_create(comp_parent);
    lv_obj_set_width(this->base, 200);
    lv_obj_set_height(this->base, 200);
    lv_obj_set_x(this->base, -182);
    lv_obj_set_y(this->base, -61);
    lv_obj_set_align(this->base, LV_ALIGN_CENTER);
    lv_obj_clear_flag(this->base, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(this->base, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(this->base, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(this->base, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(this->base, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(this->base, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(this->base, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(this->base, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(this->base, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(this->base, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    auto label_event_lambda = [](lv_event_t *e)
    {
        lv_event_code_t event_code = lv_event_get_code(e);
        lv_obj_t *target = lv_event_get_current_target(e);
        lv_msg_t *m = lv_event_get_msg(e);
        char *id = (char *)lv_msg_get_payload(m);
        TileButton *idd = (TileButton *)lv_obj_get_user_data(target);

        if (strcmp(idd->id.c_str(), id) == 0)
        {
            idd->label_event(e);
        }
    };

    lv_obj_add_event_cb(this->base, label_event_lambda, LV_EVENT_MSG_RECEIVED, this);
    lv_obj_add_event_cb(this->base, this->destruct, LV_EVENT_DELETE, this);
    lv_obj_set_user_data(this->base, this);
    lv_msg_subscribe_obj(DEV_VALUE_CHANGE, this->base, (void *)this->dev->getId().c_str());

    // Device Name
    lv_obj_t *cui_Panel5;
    cui_Panel5 = lv_obj_create(this->base);
    lv_obj_set_width(cui_Panel5, 200);
    lv_obj_set_height(cui_Panel5, 150);
    lv_obj_set_align(cui_Panel5, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(cui_Panel5, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(cui_Panel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Panel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad(cui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_DeviceName;
    cui_DeviceName = lv_label_create(cui_Panel5);
    lv_obj_set_width(cui_DeviceName, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(cui_DeviceName, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_DeviceName, 0);
    lv_obj_set_y(cui_DeviceName, 50);
    lv_obj_set_align(cui_DeviceName, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(cui_DeviceName, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    const char *devName = this->dev->getName().c_str();
    lv_label_set_text(cui_DeviceName, devName);

    auto toggle_state_lambda = [](lv_event_t *e)
    {
        TileButton *idd = (TileButton *)lv_event_get_user_data(e);

        idd->toggle_state(e);
    };

    lv_obj_add_event_cb(cui_Panel5, toggle_state_lambda, LV_EVENT_SHORT_CLICKED, this);

    auto longPressedLambda = [](lv_event_t *e)
    {
        TileButton *idd = (TileButton *)lv_event_get_user_data(e);
        idd->longPressEvent();
    };
    lv_obj_add_event_cb(cui_Panel5, longPressedLambda, LV_EVENT_LONG_PRESSED, this);

    auto icon = this->getStateIcon();
    if (icon != nullptr)
    {
        this->icon = lv_img_create(this->base);
        lv_img_set_src(this->icon, this->getStateIcon());
        lv_obj_set_width(this->icon, LV_SIZE_CONTENT);  /// 48
        lv_obj_set_height(this->icon, LV_SIZE_CONTENT); /// 48
        lv_obj_set_y(this->icon, 10);
        lv_obj_set_align(this->icon, LV_ALIGN_TOP_MID);
        lv_obj_add_flag(this->icon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
        lv_obj_clear_flag(this->icon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    }

    return this->base;
}
void TileButton::drawAdditionalElements(lv_obj_t *cui_TileButton)
{
}
lv_obj_t *TileButton::create_ui_element(lv_obj_t *comp_parent)
{
    lv_obj_t *cui_TileButton = this->drawBaseElement(comp_parent);
    this->drawAdditionalElements(cui_TileButton);
    return cui_TileButton;
}