#pragma once

class provider_settings_t
{
public:
    std::string host;
    uint16_t port;
    bool ssl = false;
    std::string apiToken;
};

namespace ArduinoJson
{
    template <>
    struct Converter<provider_settings_t>
    {
        static bool toJson(const provider_settings_t &src, JsonVariant dst)
        {

            dst["host"] = src.host;
            dst["port"] = src.port;
            dst["ssl"] = src.ssl;
            dst["apiToken"] = src.apiToken;
            return true;
        }

        static provider_settings_t fromJson(JsonVariantConst src)
        {

            provider_settings_t _settings;
            _settings.host = src["host"].as<std::string>();
            _settings.port = src["port"].as<int>();
            _settings.ssl = src["ssl"].as<bool>();
            _settings.apiToken = src["apiToken"].as<std::string>();
            return _settings;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}
