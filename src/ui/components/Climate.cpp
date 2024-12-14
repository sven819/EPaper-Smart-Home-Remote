#include "../icons/climate.h"
#include "Climate.hpp"

static const char *TAG = "ClimateButton";

const lv_img_dsc_t *ClimateButton::getStateIcon()
{
    if (this->state)
        return &climate_on;
    else
        return &climate_off;
}
