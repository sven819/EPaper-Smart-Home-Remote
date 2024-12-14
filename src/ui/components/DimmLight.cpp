#include "../icons/light.h"
#include "DimmLight.hpp"

static const char *TAG = "DimmLightButton";

const lv_img_dsc_t *DimmLightButton::getStateIcon()
{
    if (this->state)
        return &light_on;
    else
        return &light_off;
}
void DimmLightButton::click_event_cb(lv_event_t *e)
{
    lv_obj_t *clickTarget = lv_event_get_current_target(e);
    lv_obj_t **comp_TileButton = (lv_obj_t **)lv_event_get_user_data(e);

    this->value = std::max(0, std::min(100, clickTarget == this->minus ? this->value - 10 : this->value + 10));
    lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());

    // todo !
    DeviceCommand cmd;
    strcpy(cmd.device_id, this->dev->getId().c_str());
    cmd.action = static_cast<int>(DimmLight::supportedActions::setBrightness);
    ((DimmLight *)this->dev)->brightness = this->value;

    this->notify(cmd);
}

void DimmLightButton::label_event(lv_event_t *e)
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

            if (this->state != this->dev->getState())
            {
                this->state = this->dev->getState();
                lv_img_set_src(this->icon, this->getStateIcon());
            }

            if (this->value != this->dev->brightness)
            {
                this->value = this->dev->brightness;
                lv_label_set_text_fmt(this->label, "%d%s", this->value, this->unit.c_str());
            }
        }
    }
}
