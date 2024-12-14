
#include "SettingsManager.hpp"
#include <model/settings/Settings.hpp>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

static const char *TAG = "Settings";

Settings::Settings()
{
	esp_log_write(ESP_LOG_DEBUG, TAG, "SETTINGS ENTRY");
}

void Settings::begin()
{
	preferences.begin("EPDSH", false, "settings");
	String strSettings = preferences.getString("EPDSH", "");
	if (strSettings.isEmpty())
	{
		ESP_LOGV(TAG, "Load Default settings");
		loadDefaults();
	}
	else
	{
		JsonDocument doc;
		deserializeJson(doc, strSettings, DeserializationOption::NestingLimit(15));
		this->data = doc.as<settings_t>();
	}
}

void Settings::loadDefaults()
{
	settings_t _settings;
	_settings.provider = "HA";

	wifimanager_settings_t wifiSettings;

	wifiSettings.ap_ssid = "EPD-SH";
	wifiSettings.ap_password = "admin";
	_settings.wifi = wifiSettings;

	webCredentials_t webCred;
	webCred.password = "admin";
	webCred.username = "admin";
	_settings.webLogin = webCred;

	this->data = _settings;

	if (!commit())
	{
		esp_log_write(ESP_LOG_ERROR, TAG, "Speichern der Defaultwerte fehlgeschlagen");
	}
}

Settings::~Settings()
{
	this->preferences.end();
}

bool Settings::commit()
{
	JsonDocument doc;
	settings_t aa = this->data;
	doc.set(aa);

	std::string jsonOutput;
	serializeJson(doc, jsonOutput);
	Serial.println(jsonOutput.c_str());
	preferences.remove("EPDSH");
	size_t sz = preferences.putString("EPDSH", jsonOutput.c_str());
	return (sz == 0);
}

settings_t Settings::getData()
{
	return this->data;
}
void Settings::setData(settings_t set)
{
	this->data = set;
	this->commit();
}
void Settings::setupRestApi(AsyncWebServer *webserver, const char *url)
{
	auto createResponse = []() -> AsyncJsonResponse *
	{
		auto response = new AsyncJsonResponse(false);
		response->addHeader("Access-Control-Allow-Origin", "*");
		return response;
	};

	webserver->on(url, HTTP_GET, [this, createResponse](AsyncWebServerRequest *request)
				  {
        auto response = createResponse();
        JsonObject root = response->getRoot();
        const settings_t settings = this->getData();
        Converter<settings_t>().toJson(settings, root);
        response->setLength();
        request->send(response); })
		.setAuthentication(this->data.webLogin.username.c_str(), this->data.webLogin.password.c_str());

	webserver->on(url, HTTP_OPTIONS, [createResponse](AsyncWebServerRequest *request)
				  {
        auto response = createResponse();
        response->setLength();
        request->send(response); });

	AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
		url, [this](AsyncWebServerRequest *request, JsonVariant &json)
		{
            if (!request->authenticate(this->data.webLogin.username.c_str(), this->data.webLogin.password.c_str())) {
                return request->requestAuthentication();
            }
            settings_t settings = json.as<settings_t>();
			this->setData(settings);
			request->send(200); 
			vTaskDelay(200);
			ESP.restart(); });

	webserver->addHandler(handler);
}