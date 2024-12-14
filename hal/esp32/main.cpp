#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ui/Ui.hpp"
#include "Provider/ProviderFactory.hpp"
#include "HAL.hpp"
#include "SettingsManager.hpp"
#include "IoTDevs.hpp"
#include "AppState.hpp"
#include "WifiManager.hpp"
#include "PubSub.hpp"
#include <WebContent.cpp>
#include "WebServer.hpp"
#include "PowerManager.hpp"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

UI ui;
HAL hal;
Provider *provider;
WebServer webServer;
Settings settingsManager;
AppState appState;
PubSub<DeviceStatus> statusPubSub;
PubSub<DeviceCommand> commandPubSub;
PowerManager *powerManager;
DeviceStore store(&statusPubSub);
WiFiManager *wifiManager;

settings_t settings;

bool checkChanges();
void appShutdown();
void setupWebserver(void);
void setupUI(const settings_t &settings, bool drawOnce);
bool initializeProvider(const settings_t &settings, bool timerWakeup);

void setup(void)
{
	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	esp_log_level_set("*", ESP_LOG_VERBOSE);
	wifiManager = new WiFiManager(&WiFi);
	settingsManager.begin();
	settings = settingsManager.getData();
	wifiManager->setup(&settings.wifi);

	powerManager = new PowerManager(appShutdown);

	bool synchronStart = appState.wakeup_reason == ESP_SLEEP_WAKEUP_TIMER;

	if (hal.factoryResetEnter())
		settingsManager.loadDefaults();

	hal.init(synchronStart);

	wifiManager->begin(synchronStart);

	store.initFromSettings(settings.devices);

	provider = ProviderFactory::createProvider(settings.provider, &store, settings.providerParams, &commandPubSub);
	provider->begin(synchronStart);

	if (synchronStart && checkChanges())
	{ // todo Powermanager
		hal.startSleep();
	}
	else
	{
		setupUI(settings, synchronStart);
		setupWebserver();
	}
}

void loop(void)
{
	vTaskDelete(NULL);
}
// Checksum diff check
bool checkChanges()
{
	std::string viewname = appState.getUiState();
	uint32_t chkSum = store.getChecksum(settings.getDevicesByView(viewname));
	return (chkSum == appState.getChecksum());
}
// Shutdown Task PowerManager
void appShutdown()
{
	provider->end();
	std::string viewname = appState.getUiState();
	uint32_t chkSum = store.getChecksum(settings.getDevicesByView(viewname));
	appState.setChecksum(chkSum);
	hal.startSleep();
}

void setupWebserver(void)
{
	webServer.registerRestApi(&settingsManager, "/api/settings");
	webServer.registerRestApi(wifiManager, "/api/wifi");
	WebContent::setupStaticWebContent(webServer.getServer());
	webServer.begin();
}

void setupUI(const settings_t &settings, bool drawOnce)
{
	ui.init(settings.views, &store, &statusPubSub, &commandPubSub, &hal);
	AppState state = appState;
	ui.setStateChangeCallback([&state](const std::string &message)
							  { state.setUiState(message); });
	ui.draw(state.getUiState(), drawOnce);
	if (drawOnce)
		hal.startSleep(); // todo PWR Manager
}