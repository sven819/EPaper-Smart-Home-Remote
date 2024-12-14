#pragma once
#include "IoTDevs.hpp"
#include "../model/settings/provider.hpp"
#include "Provider.hpp"
#include "esp_websocket_client.h"
#include "HaModel.hpp"

class HaProvider : public Provider
{
public:
    void toggleState(bool on, std::string *id);
    HaProvider(DeviceStore *store, provider_settings_t settings, PubSub<DeviceCommand> *_commandPubSub);
    void handle();
    bool online();
    bool end() override;

private:
    JsonDocument revBuffer;
    void websocket_event_handler(esp_event_base_t base, int32_t event_id, void *event_data);

    esp_websocket_client_handle_t client;
    bool _online = false;
    void sendAuthMessage();
    void aboDevices();

    void serviceCall(const char *entity_id, const char *domain, const char *service, JsonDocument *serviceData = nullptr);
    provider_settings_t settings;
    int msgId;
    int initRequest;
    void HaProviderDev(IoTDevice *obj, JsonVariant data);

    template <typename T>
    void process(T *obj, DeviceCommand cmd);

    template <typename T>
    void event(T *obj, EntityState data)
    {
        static_assert(std::is_base_of<IoTDevice, T>::value, "T must be a derived class of Base");
        ESP_LOGI("HaProvider", "General event for type: %s", typeid(T).name());
        IoTDevice *dev = obj;

        dev->setChecksum(data.lastChange);

        if (obj != nullptr)
        {
            if (ColorLight *d = dynamic_cast<ColorLight *>(obj); d != nullptr)
            {
                this->event<ColorLight>(d, data);
            }
            else if (DimmLight *d = dynamic_cast<DimmLight *>(obj); d != nullptr)
            {
                this->event<DimmLight>(d, data);
            }
            else if (Light *d = dynamic_cast<Light *>(obj); d != nullptr)
            {
                this->event<Light>(d, data);
            }
            else if (Sensor *d = dynamic_cast<Sensor *>(obj); d != nullptr)
            {
                this->event<Sensor>(d, data);
            }
            else if (Climate *d = dynamic_cast<Climate *>(obj); d != nullptr)
            {
                this->event<Climate>(d, data);
            }
            else if (Shutter *d = dynamic_cast<Shutter *>(obj); d != nullptr)
            {
                this->event<Shutter>(d, data);
            }
            else
            {
                ESP_LOGE("HaProvider", "General processing for unkown type!");
            }
        }
    }
};
