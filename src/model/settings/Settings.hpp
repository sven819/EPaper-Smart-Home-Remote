#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <ArduinoJson.h>

#include "StlConverter.hpp"
#include "device.hpp"
#include "Views.hpp"
#include "WifiManager.hpp"
#include "provider.hpp"

struct webCredentials_t
{
    std::string username;
    std::string password;
};

namespace ArduinoJson
{
    template <>
    struct Converter<webCredentials_t>
    {
        static bool toJson(const webCredentials_t &src, JsonVariant dst)
        {

            dst["username"] = src.username;
            dst["password"] = src.password;

            return true;
        }

        static webCredentials_t fromJson(JsonVariantConst src)
        {

            webCredentials_t _settings;
            _settings.username = src["username"].as<std::string>();
            _settings.password = src["password"].as<std::string>();
            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
            // return src["real"].is<double>() && src["imag"].is<double>();
        }
    };
}

struct settings_t
{
    std::string provider = "HA";
    std::vector<device_t> devices;
    wifimanager_settings_t wifi;
    provider_settings_t providerParams;
    std::vector<view_t> views;
    webCredentials_t webLogin;
    std::vector<std::string> getDevicesByView(const std::string &viewname) const
    {
        auto it = std::find_if(this->views.begin(), this->views.end(),
                               [viewname](const view_t &obj)
                               {
                                   return obj.name == viewname;
                               });

        if (it != this->views.end())
        {
            const view_t &foundView = *it;
            return foundView.devices;
        }
        return std::vector<std::string>();
    };
};

namespace ArduinoJson
{
    template <>
    struct Converter<settings_t>
    {
        static bool toJson(const settings_t &src, JsonVariant dst)
        {

            dst["provider"] = src.provider;
            dst["devices"] = src.devices;
            dst["wifi"] = src.wifi;
            dst["views"] = src.views;
            dst["providerParams"] = src.providerParams;
            dst["webLogin"] = src.webLogin;

            return true;
        }

        static settings_t fromJson(JsonVariantConst src)
        {

            settings_t _settings;
            _settings.provider = src["provider"].as<std::string>();
            _settings.devices = src["devices"].as<std::vector<device_t>>();
            _settings.wifi = src["wifi"].as<wifimanager_settings_t>();
            _settings.views = src["views"].as<std::vector<view_t>>();
            _settings.providerParams = src["providerParams"].as<provider_settings_t>();
            _settings.webLogin = src["webLogin"].as<webCredentials_t>();

            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}