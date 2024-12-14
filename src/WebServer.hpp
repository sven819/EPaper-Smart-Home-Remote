#include <ESPAsyncWebServer.h>
#include "IRestApiSetup.hpp"
class WebServer
{
public:
    WebServer();
    ~WebServer();
    void registerRestApi(IRestApiSetup *apiSetupInstance, const char *url);

    void begin();
    void on(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
    void serveStatic(const char *uri, fs::FS &filesystem, const char *path, const char *cache_control = NULL);
    AsyncWebServer *getServer();

private:
    AsyncWebServer server;
};