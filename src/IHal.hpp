#pragma once
#include <time.h>

class IHalInterface
{
public:
    virtual ~IHalInterface() = default;
    virtual void init(bool synchronStart) = 0;

    virtual tm getTime() = 0;

    virtual uint16_t batteryState()
    {

        return 0;
    }
    virtual bool networkConnected() = 0;

    virtual void startSleep() = 0;
    virtual bool factoryResetEnter() { return false; };
    bool displayReady = false;
};