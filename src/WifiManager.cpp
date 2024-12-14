#include "WiFiManager.hpp"

#include <ArduinoJson.h>
#include "WiFiUdp.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include "NTP.h"
#include <AsyncJson.h>

#define checkInterval 10000

static const char *TAG = "WiFI";

WiFiManager::WiFiManager(WiFiClass *wifi)
    : _wifi(wifi), _settings(nullptr)
{
    this->_wifi->begin();

    wifi->onEvent([](arduino_event_id_t event, arduino_event_info_t info)
                  {
        system_event_sta_connected_t e = info.wifi_sta_connected;
        esp_log_write(ESP_LOG_INFO, TAG, "Connected to %s\r\n", (char *)e.ssid); },
                  ARDUINO_EVENT_WIFI_STA_CONNECTED);

    wifi->onEvent([this](arduino_event_id_t event, arduino_event_info_t info)
                  {
                      system_event_sta_got_ip_t e = info.got_ip;
                      esp_log_write(ESP_LOG_INFO, TAG, "Got IP: %s\r\n", IPAddress(e.ip_info.ip.addr).toString().c_str());
                      esp_log_write(ESP_LOG_INFO, TAG, "Got NM: %s\r\n", IPAddress(e.ip_info.netmask.addr).toString().c_str());
                      esp_log_write(ESP_LOG_INFO, TAG, "Got GW: %s\r\n", IPAddress(e.ip_info.gw.addr).toString().c_str()); },
                  ARDUINO_EVENT_WIFI_STA_GOT_IP);
    wifi->onEvent([](arduino_event_id_t event, arduino_event_info_t info)
                  {
        system_event_sta_disconnected_t e = info.wifi_sta_disconnected;
        esp_log_write(ESP_LOG_INFO, TAG, "Disconnected from SSID: %s\tReason: %d\r\n", ((char *)e.ssid), e.reason); },
                  ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    wifi->onEvent([](arduino_event_id_t event, arduino_event_info_t info)
                  {
        system_event_ap_probe_req_rx_t e = info.wifi_ap_probereqrecved;
        esp_log_write(ESP_LOG_INFO, TAG, "Probe received: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X (%d)\r\n", e.mac[0], e.mac[1], e.mac[2], e.mac[3], e.mac[4], e.mac[5], e.rssi); },
                  ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED);
    wifi->onEvent([](arduino_event_id_t event, arduino_event_info_t info)
                  {
        system_event_ap_staconnected_t e = info.wifi_ap_staconnected;
        esp_log_write(ESP_LOG_INFO, TAG, "Client %.2X:%.2X:%.2X:%.2X:%.2X:%.2X (%d) connected\r\n", e.mac[0], e.mac[1], e.mac[2], e.mac[3], e.mac[4], e.mac[5], e.aid); },
                  ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    wifi->onEvent([](arduino_event_id_t event, arduino_event_info_t info)
                  {
        system_event_ap_stadisconnected_t e = info.wifi_ap_stadisconnected;
        esp_log_write(ESP_LOG_INFO, TAG, "Client %.2X:%.2X:%.2X:%.2X:%.2X:%.2X (%d) disconnected\r\n", e.mac[0], e.mac[1], e.mac[2], e.mac[3], e.mac[4], e.mac[5], e.aid); },
                  ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
}

WiFiManager *WiFiManager::setup(const wifimanager_settings_t *settings)
{
    this->_settings = settings;

    //  this->_wifi->mode(WIFI_MODE_STA);
    //  this->_wifi->setSleep(false);
    //  this->_wifi->setAutoReconnect(true);

    /*   this->ntp = new NTP(this->wifiUdp);
      this->ntp->ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
      this->ntp->ruleSTD("CET", Last, Sun, Oct, 3, 60);   // last sunday in october 3:00, timezone +60min (+1 GMT)
      this->ntp->begin("0.de.pool.ntp.org"); */
    // this->_wifi->setHostname(hostname);
    // this->_wifi->softAPsetHostname(hostname);
    return this;
}

bool WiFiManager::begin(bool blocking)
{

    // this->end();
    if (this->_wifi->status() == WL_CONNECTED)
    {
        ESP_LOGI(TAG, "WIFI Connected");
        return true;
    }
    if (this->_settings->sta_ssid.length() > 0 && this->_settings->sta_password.length() > 0)
    {
        ESP_LOGI(TAG, "WIFI not Connected");

        esp_log_write(ESP_LOG_DEBUG, TAG, "Connect STA");
        if (strcmp(this->_wifi->SSID().c_str(), this->_settings->sta_ssid.c_str()) != 0 ||
            strcmp(this->_wifi->psk().c_str(), this->_settings->sta_password.c_str()) != 0)
        {
            ESP_LOGI(TAG, "WIFI set Config : %s %s", this->_wifi->SSID().c_str(), this->_wifi->psk().c_str());

            this->end();
            this->_wifi->mode(WIFI_STA);
            this->_wifi->persistent(true);
            this->_wifi->setAutoConnect(true);
            this->_wifi->begin(this->_settings->sta_ssid.c_str(), this->_settings->sta_password.c_str());
            // this->_wifi->persistent(false);
        }
    }

    else
    {
        this->_wifi->mode(WIFI_MODE_STA);
        this->_wifi->disconnect();
    }

    if ((this->_wifi->getMode() & WIFI_MODE_AP) == WIFI_MODE_AP)
    {
        esp_log_write(ESP_LOG_DEBUG, TAG, "ap.ssid: %s\r\n", this->_settings->ap_ssid);
        esp_log_write(ESP_LOG_VERBOSE, TAG, "ap.password: %s\r\n", this->_settings->ap_password);
        this->_wifi->softAP(this->_settings->ap_ssid.c_str(), this->_settings->ap_password.c_str());
    }
    else
    {
        this->_wifi->softAPdisconnect(true);
    }

    auto wifiTaskLambda = [](void *pvParameters)
    {
        auto *instance = static_cast<WiFiManager *>(pvParameters);
        for (;;)
        {
            vTaskDelay(10000);
            instance->handle();
        }
    };
    BaseType_t testTask = xTaskCreatePinnedToCore(wifiTaskLambda, "wifiTask", 2000, this, 10, NULL, 0);
    if (blocking)
    {
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            ESP_LOGE(TAG, "WiFi Failed!\n");
        }
    }

    return this->_wifi->getMode() != WIFI_MODE_NULL;
}

void WiFiManager::end()
{
    TaskHandle_t taskHandle = xTaskGetHandle("wifiTask");
    if (taskHandle != NULL)
    {
        vTaskDelete(taskHandle);
    }
    this->_wifi->disconnect();
    this->_wifi->softAPdisconnect();
}

void WiFiManager::getNTP()
{
    ESP_LOGI(TAG, "GET NTP");
    if (!this->ntp->update())
        ESP_LOGE(TAG, "NTP ERROR"); // Www hh:mm:ss

    const char *dt = this->ntp->formattedTime("%A %T");
    ESP_LOGI(TAG, "%s", dt); // Www hh:mm:ss
}

void WiFiManager::handle()
{
    if (!this->_wifi->isConnected() && (this->_wifi->getMode() & WIFI_MODE_AP) != WIFI_MODE_AP)
    {

        //  esp_log_write(ESP_LOG_DEBUG, TAG, "STA is connected\r\n");
        this->_wifi->mode(WIFI_MODE_AP);
        this->_wifi->softAP(this->_settings->ap_ssid.c_str(), this->_settings->ap_password.c_str());
    }
}

void WiFiManager::setupRestApi(AsyncWebServer *webserver, const char *url)
{

    webserver->on(url, HTTP_GET, [this](AsyncWebServerRequest *request)
                  {
                      AsyncJsonResponse *response = new AsyncJsonResponse(true);
                      response->addHeader("Access-Control-Allow-Origin", "*");

                      JsonArray root = response->getRoot();

                      int n = WiFi.scanComplete();
                      if (n > 0) 
                      {
                          for (int i = 0; i < n; ++i)
                          {
                              JsonObject jsNetwork = root.createNestedObject();
                              jsNetwork["SSID"] = WiFi.SSID(i);
                              jsNetwork["RSSI"] = WiFi.RSSI(i);
                          }
                      }

                      response->setLength();
                      request->send(response); });
}
