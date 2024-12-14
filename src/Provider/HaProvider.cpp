#include "HaProvider.hpp"
#include <ArduinoJson.h>
#include "IoTDevs.hpp"
#include "../PubSub.hpp"
#include "esp_websocket_client.h"
#include "HaModel.hpp"

static const char *TAG = "HAProvider";

using namespace std::placeholders;

bool HaProvider::end()
{
    this->_online = false;
    return esp_websocket_client_close(this->client, portMAX_DELAY);
};

void HaProvider::serviceCall(const char *entity_id, const char *domain, const char *service, JsonDocument *serviceData)
{

    JsonDocument doc;
    doc["id"] = this->msgId++;
    doc["type"] = "call_service";
    doc["domain"] = domain;
    doc["service"] = service;
    doc["service_data"]["entity_id"] = entity_id;
    if (serviceData != nullptr)
    {
        for (JsonPair kv : serviceData->as<JsonObject>())
        {
            doc["service_data"][kv.key().c_str()] = kv.value();
        }
    }
    std::string output;
    serializeJson(doc, output);
    esp_websocket_client_send_text(this->client, output.c_str(), output.length(), portMAX_DELAY);
}

void HaProvider::handle()
{
    DeviceCommand command;
    if (this->commandPubSub->receive(this->clientID, command, portMAX_DELAY))
    {
        IoTDevice *dev = this->store->getDevice(command.device_id);
        this->process(dev, command);
    }
}

void HaProvider::websocket_event_handler(esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = static_cast<esp_websocket_event_data_t *>(event_data);

    if (!data)
    {
        ESP_LOGE(TAG, "Event data is null");
        return;
    }

    if (event_id == WEBSOCKET_EVENT_CONNECTED)
    {
        this->sendAuthMessage();
    }
    else if (event_id == WEBSOCKET_EVENT_DATA && data->data_ptr != nullptr)
    {

        DeserializationError error = deserializeJson(this->revBuffer, data->data_ptr);

        if (error)
        {
            ESP_LOGE(TAG, "deserializeJson() failed: %s", error.c_str());
            ESP_LOGE(TAG, "Free heap size: %d bytes", esp_get_free_heap_size());
            return;
        }
        serializeJsonPretty(this->revBuffer, Serial);

        const char *type = this->revBuffer["type"];
        if (strcmp("auth_ok", type) == 0)
        {
            this->aboDevices();
        }
        else if (strcmp("event", type) == 0)
        {
            std::vector<EntityState> states = this->revBuffer["event"].as<std::vector<EntityState>>();
            for (EntityState state : states)
            {
                IoTDevice *dev = this->store->getDevice(state.id);
                this->event(dev, state);
            }
            if (!this->_online && revBuffer["id"].as<int>() == this->initRequest)
            {
                this->_online = true;
                ESP_LOGV(TAG, "HA Provider Online");
            }
        }
    }
}
HaProvider::HaProvider(DeviceStore *store, provider_settings_t _settings, PubSub<DeviceCommand> *_commandPubSub) : Provider(store, _settings, _commandPubSub)
{
    ESP_LOGI(TAG, "Create HA Provider");
    this->_online = false;
    this->settings = _settings;
    this->msgId = 1;

    const esp_websocket_client_config_t websocket_cfg = {.host = this->settings.host.c_str(), .port = this->settings.port, .path = "/api/websocket", .buffer_size = 2048};

    this->client = esp_websocket_client_init(&websocket_cfg);
    auto lambda_websocket_event_handler = [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
        HaProvider *instance = static_cast<HaProvider *>(handler_args);
        if (instance)
        {
            instance->websocket_event_handler(base, event_id, event_data);
        }
    };
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, lambda_websocket_event_handler, (void *)this);
    esp_websocket_client_start(client);
}

void HaProvider::sendAuthMessage()
{
    JsonDocument doc;

    doc["type"] = "auth";
    doc["access_token"] = this->settings.apiToken.c_str();

    std::string payload;
    serializeJson(doc, payload);
    esp_websocket_client_send_text(this->client, payload.c_str(), payload.length(), portMAX_DELAY);
}
void HaProvider::aboDevices()
{
    JsonDocument doc;
    this->initRequest = this->msgId++;
    doc["id"] = this->initRequest;
    doc["type"] = "subscribe_entities";
    doc["entity_ids"].set(this->store->getDevices());
    std::string output;
    serializeJson(doc, output);
    esp_websocket_client_send_text(this->client, output.c_str(), output.length(), portMAX_DELAY);
}

bool HaProvider::online()
{
    return this->_online;
}

template <typename T>
void HaProvider::process(T *obj, DeviceCommand cmd)
{

    static_assert(std::is_base_of<IoTDevice, T>::value, "T must be a derived class of Base");
    ESP_LOGI("HaProvider", "General processing for type: %s", typeid(T).name());
    if (ColorLight *colorLight = dynamic_cast<ColorLight *>(obj))
    {
        this->process<ColorLight>(colorLight, cmd);
    }
    else if (DimmLight *dimmLight = dynamic_cast<DimmLight *>(obj))
    {
        this->process<DimmLight>(dimmLight, cmd);
    }
    else if (Light *light = dynamic_cast<Light *>(obj))
    {
        this->process<Light>(light, cmd);
    }
    else if (Climate *climate = dynamic_cast<Climate *>(obj))
    {
        this->process<Climate>(climate, cmd);
    }
    else if (Sensor *sensor = dynamic_cast<Sensor *>(obj))
    {
        this->process<Sensor>(sensor, cmd);
    }
    else if (Shutter *shutter = dynamic_cast<Shutter *>(obj))
    {
        this->process<Shutter>(shutter, cmd);
    }
}

template <>
void HaProvider::process<Light>(Light *obj, DeviceCommand cmd)
{

    ESP_LOGI(TAG, "LIGHT Prozess Action %d", cmd.action);
    IoTDevice::supportedActions action = static_cast<IoTDevice::supportedActions>(cmd.action);
    if (action == IoTDevice::supportedActions::setState)
    {
        ESP_LOGI(TAG, "SetState ACTION");
        const char *service = !obj->getState() ? "turn_on" : "turn_off";
        this->serviceCall(obj->getId().c_str(), "light", service);
    }
}

template <>
void HaProvider::process<DimmLight>(DimmLight *obj, DeviceCommand cmd)
{

    ESP_LOGI(TAG, "LIGHT Prozess Action %d", cmd.action);
    DimmLight::supportedActions action = static_cast<DimmLight::supportedActions>(cmd.action);
    if (action == DimmLight::supportedActions::setBrightness)
    {
        JsonDocument srvData;
        srvData["brightness"] = (int)(obj->brightness * 2.55);
        this->serviceCall(obj->getId().c_str(), "light", "turn_on", &srvData);
    }
    else
        this->process<Light>((Light *)obj, cmd);
}

template <>
void HaProvider::process<ColorLight>(ColorLight *obj, DeviceCommand cmd)
{

    ESP_LOGI(TAG, "LIGHT Prozess Action %d", cmd.action);
    ColorLight::supportedActions action = static_cast<ColorLight::supportedActions>(cmd.action);
    if (action == ColorLight::supportedActions::setColor)
    {

        JsonDocument srvData;
        srvData["rgb_color"] = obj->color.asVector();
        this->serviceCall(obj->getId().c_str(), "light", "turn_on", &srvData);
    }
    else
        this->process<DimmLight>((DimmLight *)obj, cmd);
}

template <>
void HaProvider::process<Climate>(Climate *obj, DeviceCommand cmd)
{
    IoTDevice::supportedActions action = static_cast<IoTDevice::supportedActions>(cmd.action);
    if (action == IoTDevice::supportedActions::setState)
    {
        ESP_LOGI(TAG, "SetState ACTION");
    }
    std::string mode = !obj->getState() ? "auto" : "off";
    JsonDocument srvData;
    srvData["hvac_mode"] = mode;
    this->serviceCall(obj->getId().c_str(), "climate", "set_hvac_mode", &srvData);
    // kein Response, Polling ?
    obj->setState(!obj->getState());
}

template <>
void HaProvider::process<Shutter>(Shutter *obj, DeviceCommand cmd)
{

    ESP_LOGI(TAG, "LIGHT Prozess Action %d", cmd.action);
    Shutter::supportedActions action = static_cast<Shutter::supportedActions>(cmd.action);
    JsonDocument srvData;

    if (action == Shutter::supportedActions::setHeight)
    {
        srvData["position"] = 100 - obj->height;
    }
    else
    {
        srvData["position"] = obj->getState() ? 100 : 0;
    }

    this->serviceCall(obj->getId().c_str(), "cover", "set_cover_position", &srvData);
}

template <>
void HaProvider::event<Light>(Light *obj, EntityState data)
{
    if (!data.state.empty())
        this->store->setState(obj, data.boolState());
}

template <>
void HaProvider::event<DimmLight>(DimmLight *obj, EntityState data)
{

    if (int brightness; data.getIntAttribute("brightness", &brightness))
    {
        // Attribute in HA 0-255
        obj->brightness = brightness / 2.55;
    };
    if (!data.state.empty())
        this->store->setState(obj, data.boolState());

    ESP_LOGI(TAG, "Brightness : %d", obj->brightness);
}

template <>
void HaProvider::event<ColorLight>(ColorLight *obj, EntityState data)
{
    this->event<DimmLight>(obj, data);
}

template <>
void HaProvider::event<Climate>(Climate *obj, EntityState data)
{
    if (!data.state.empty())
        this->store->setState(obj, data.boolState());
}

template <>
void HaProvider::event<Sensor>(Sensor *obj, EntityState data)
{
    if (!data.state.empty())
        obj->value = data.state;
    data.getStringAttribute("unit_of_measurement", &obj->unit);
    this->store->notifyObservers(*obj);
}
template <>
void HaProvider::event<Shutter>(Shutter *obj, EntityState data)
{
    if (!data.state.empty())
        this->store->setState(obj, data.boolState());
    if (int position; data.getIntAttribute("current_position", &position))
    {
        obj->height = 100 - position;
    };
    this->store->notifyObservers(*obj);
}