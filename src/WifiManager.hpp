#pragma once
#include <ArduinoJson.h>
#include <WiFi.h>
#include "NTP.h"
#include "IRestApiSetup.hpp"

struct wifimanager_settings_t
{
    std::string ap_ssid = "EPD_SH";
    std::string ap_password = "EPD_SH";
    std::string sta_ssid = "EPD_SH";
    std::string sta_password = "EPD_SH";
};

namespace ArduinoJson
{
    template <>
    struct Converter<wifimanager_settings_t>
    {
        static bool toJson(const wifimanager_settings_t &src, JsonVariant dst)
        {
            dst["ap_ssid"] = src.ap_ssid;
            dst["ap_password"] = src.ap_password;
            dst["sta_ssid"] = src.sta_ssid;
            dst["sta_password"] = src.sta_password;

            return true;
        }

        static wifimanager_settings_t fromJson(JsonVariantConst src)
        {
            wifimanager_settings_t _settings;
            _settings.ap_ssid = src["ap_ssid"].as<std::string>();
            _settings.ap_password = src["ap_password"].as<std::string>();
            _settings.sta_ssid = src["sta_ssid"].as<std::string>();
            _settings.sta_password = src["sta_password"].as<std::string>();
            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}

class WiFiManager : public IRestApiSetup
{
public:
    WiFiManager(WiFiClass *wifi);
    WiFiManager *setup(const wifimanager_settings_t *settings);
    WiFiClass *wifi() { return this->_wifi; }
    bool begin(bool blocking = false);
    void end();
    void handle();
    void getNTP();
    void setupRestApi(AsyncWebServer *webserver, const char *url) override;

private:
    WiFiClass *_wifi;
    WiFiUDP wifiUdp;
    NTP *ntp;
    const wifimanager_settings_t *_settings;
    wifi_mode_t mode;
};
