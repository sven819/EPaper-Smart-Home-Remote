#include "../icons/light.h"
#include "Light.hpp"

static const char *TAG = "LightButton";

const lv_img_dsc_t *LightButton::getStateIcon()
{
    if (this->state)
        return &light_on;
    else
        return &light_off;
}
void LightButton::drawAdditionalElements(lv_obj_t *cui_TileButton)
{
    return;
}