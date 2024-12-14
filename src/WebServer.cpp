
#include "WebServer.hpp"
#include "IRestApiSetup.hpp"

WebServer::WebServer() : server(80)
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
}

WebServer::~WebServer()
{
}

void WebServer::begin()
{
    server.begin();
}

void WebServer::registerRestApi(IRestApiSetup *apiSetupInstance, const char *url)
{
    if (apiSetupInstance != nullptr)
    {
        apiSetupInstance->setupRestApi(&server, url);
    }
}

AsyncWebServer *WebServer::getServer()
{
    return &server;
}