#pragma once
#include "IoTDevs.hpp"
#include "../PubSub.hpp"

class Provider
{
public:
    Provider(DeviceStore *store, provider_settings_t settings, PubSub<DeviceCommand> *_commandPubSub)
    {
        this->store = store;
        this->commandPubSub = _commandPubSub;
        this->clientID = this->commandPubSub->subscribe();
    };
    virtual void handle() = 0;
    virtual bool online() = 0;
    virtual bool end() = 0;
    bool begin(bool blocking)
    {
        auto networkTaskLambda = [](void *pvParameters)
        {
            if (pvParameters == nullptr)
            {
                ESP_LOGE("PROVIDER", "Provider Config error");
            }
            TickType_t xLastWakeTime = xTaskGetTickCount();
            const TickType_t xFrequency = pdMS_TO_TICKS(20);

            auto *instance = static_cast<Provider *>(pvParameters);
            for (;;)
            {
                if (instance != nullptr)
                {

                    instance->handle();
                }
                vTaskDelayUntil(&xLastWakeTime, xFrequency);
            }
        };

        BaseType_t _networkTask = xTaskCreatePinnedToCore(networkTaskLambda, "networkTask", 10000, this, 1, NULL, 0);

        if (_networkTask != pdPASS)
            return false;

        while (blocking && !(this->online()))
        {
            vTaskDelay(50);
        }
        return true;
    }

protected:
    DeviceStore *store;
    PubSub<DeviceCommand> *commandPubSub;
    int clientID;
};
