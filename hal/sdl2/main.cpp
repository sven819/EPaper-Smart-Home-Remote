#include <lvgl.h>
#include "app_hal.h"
#include "ui/Ui.h"
#include "Broker.h"
#include <stdio.h>

UI ui;

void setup()
{

  lv_init();
  hal_setup();
  ui.draw();
}
void loop()
{

  hal_loop();
}
