#include "../icons/light.h"
#include "Media.hpp"

static const char *TAG = "MediaButton";

const lv_img_dsc_t *MediaButton::getStateIcon()
{
    if (this->state)
        return &light_on;
    else
        return &light_off;
}
void MediaButton::drawAdditionalElements(lv_obj_t *cui_TileButton)
{
    return;
}