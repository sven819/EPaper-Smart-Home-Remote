#include "../icons/shutter.h"
#include "Shutter.hpp"

static const char *TAG = "ShutterButton";

const lv_img_dsc_t *ShutterButton::getStateIcon()
{
    if (this->state)
        return &shutter_on;
    else
        return &shutter_off;
}
void ShutterButton::click_event_cb(lv_event_t *e)
{
    lv_obj_t *clickTarget = lv_event_get_current_target(e);
    lv_obj_t **comp_TileButton = (lv_obj_t **)lv_event_get_user_data(e);

    this->value = std::max(0, std::min(100, clickTarget == this->minus ? this->value - 10 : this->value + 10));
    lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());

    // todo !
    DeviceCommand cmd;
    strcpy(cmd.device_id, this->dev->getId().c_str());
    cmd.action = static_cast<int>(Shutter::supportedActions::setHeight);
    this->dev->height = this->value;

    this->notify(cmd);
}

void ShutterButton::label_event(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_msg_t *m = lv_event_get_msg(e);
    char *id = (char *)lv_msg_get_payload(m);

    if (strcmp(this->id.c_str(), id) == 0)
    {
        if (event_code == LV_EVENT_MSG_RECEIVED)
        {

            if (this->state != dev->getState())
            {
                this->state = dev->getState();
                lv_img_set_src(this->icon, this->getStateIcon());
            }

            if (this->value != this->dev->height)
            {
                this->value = this->dev->height;
                lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());
            }
        }
    }
}
