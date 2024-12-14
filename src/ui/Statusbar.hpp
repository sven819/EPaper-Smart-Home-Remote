#pragma once
class Statusbar
{
public:
    Statusbar(IHalInterface *hal, lv_obj_t *parent)
    {
        this->hal = hal;
        // Statusbar erstellen
        lv_obj_t *statusbar = lv_obj_create(parent);
        lv_obj_set_width(statusbar, LV_PCT(100));
        lv_obj_set_height(statusbar, 50); // Höhe der Statusleiste anpassen
        lv_obj_set_style_pad_right(statusbar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusbar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusbar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Akkuanzeige erstellen
        this->battery_label = lv_label_create(statusbar);
        // lv_label_set_text(battery_label, LV_SYMBOL_BATTERY_3);
        lv_obj_align(battery_label, LV_ALIGN_LEFT_MID, 5, 0); // Links in der Statusleiste positionieren

        // Uhrzeit erstellen
        this->time_label = lv_label_create(statusbar);
        lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0); // In der Mitte der Statusleiste positionieren

        // WiFi-Signal erstellen
        this->wifi_label = lv_label_create(statusbar);
        lv_obj_align(wifi_label, LV_ALIGN_RIGHT_MID, -15, 0); // Rechts in der Statusleiste positionieren

        this->update();
    };

    void update()
    {
        lv_label_set_text(wifi_label, getWifiLabel());

        const tm time = this->hal->getTime();

        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M", &time);
        lv_label_set_text(time_label, buffer);
        lv_label_set_text(battery_label, getBatteryLabel());
    }
    const char *getBatteryLabel()
    {
        uint16_t state = this->hal->batteryState();
        if (state >= 77)
        {
            return LV_SYMBOL_BATTERY_FULL;
        }
        else if (state >= 55)
        {
            return LV_SYMBOL_BATTERY_3;
        }
        else if (state >= 33)
        {
            return LV_SYMBOL_BATTERY_2;
        }
        else if (state >= 15)
        {
            return LV_SYMBOL_BATTERY_1;
        }
        else
        {
            return LV_SYMBOL_BATTERY_EMPTY;
        }
    }
    const char *getWifiLabel()
    {
        if (hal->networkConnected())
            return LV_SYMBOL_WIFI;
        return LV_SYMBOL_WARNING;
    }

private:
    lv_obj_t *battery_label;
    lv_obj_t *time_label;
    lv_obj_t *wifi_label;
    IHalInterface *hal;
};
