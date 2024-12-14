#pragma once

#include <esp_wifi_types.h>
#include <iostream>
#include <map>
#include <vector>
#include <ArduinoJson.h>

struct device_t
{
    std::string name;
    std::string id;
    std::string type;
};

namespace ArduinoJson
{
    template <>
    struct Converter<device_t>
    {
        static bool toJson(const device_t &src, JsonVariant dst)
        {
            dst["name"] = src.name;
            dst["type"] = src.type;
            dst["id"] = src.id;

            return true;
        }

        static device_t fromJson(JsonVariantConst src)
        {
            device_t _settings;
            _settings.name = src["name"].as<std::string>();
            _settings.type = src["type"].as<std::string>();
            _settings.id = src["id"].as<std::string>();
            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}
