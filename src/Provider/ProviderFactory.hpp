#pragma once
#include "HaProvider.hpp"
#include "Provider.hpp"

class ProviderFactory
{
public:
    static Provider *createProvider(const std::string &type, DeviceStore *store, provider_settings_t settings, PubSub<DeviceCommand> *_commandPubSub)
    {
        if (type == "HA")
        {
            return new HaProvider(store, settings, _commandPubSub);
        }
        else
        {
            return nullptr;
        }
    }
};