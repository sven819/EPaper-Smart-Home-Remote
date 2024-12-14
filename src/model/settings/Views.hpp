#pragma once

#include <vector>
#include <ArduinoJson.h>

#include "StlConverter.hpp"

struct view_t
{
    std::string name;
    std::vector<std::string> devices;
};

namespace ArduinoJson
{
    template <>
    struct Converter<view_t>
    {
        static bool toJson(const view_t &src, JsonVariant dst)
        {
            dst["name"] = src.name;
            dst["devices"] = src.devices;
            return true;
        }

        static view_t fromJson(JsonVariantConst src)
        {
            view_t _settings;
            _settings.name = src["name"].as<std::string>();
            _settings.devices = src["devices"].as<std::vector<std::string>>();
            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}
