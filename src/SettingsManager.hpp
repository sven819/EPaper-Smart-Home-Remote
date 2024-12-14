#include <model/settings/Settings.hpp>
#include <Preferences.h>
#include <CRC32.h>
#include <ESPAsyncWebServer.h>
#include "IRestApiSetup.hpp"

class Settings : public IRestApiSetup
{
private:
    settings_t data;
    Preferences preferences;

public:
    Settings();
    ~Settings();
    settings_t getData();
    void begin();
    void setData(settings_t set);
    void loadDefaults();
    bool commit();
    void setupRestApi(AsyncWebServer *webserver, const char *url) override;
};
