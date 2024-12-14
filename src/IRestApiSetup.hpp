#pragma once

#include <ESPAsyncWebServer.h>

class IRestApiSetup
{
public:
    virtual ~IRestApiSetup() = default;
    virtual void setupRestApi(AsyncWebServer *webserver, const char *url) = 0;
};