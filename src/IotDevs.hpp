#pragma once

#include <variant>
#include <string>
#include <array>
#include <memory>
#include <iostream>
#include "model/settings/Settings.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include "PubSub.hpp"
#include <CRC32.h>
#include <list>

typedef struct
{
    char device_id[80];
} DeviceStatus;

class DeviceCommand
{
public:
    char device_id[80];
    int action; // enum class supportedActions
};

class RGB
{
public:
    RGB(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}

    uint8_t getRed() const { return red; }
    uint8_t getGreen() const { return green; }
    uint8_t getBlue() const
    {

        return blue;
    }
    std::array<uint8_t, 3> asArray() const
    {
        return {red, green, blue};
    }
    std::vector<uint8_t> asVector() const
    {
        return {red, green, blue};
    }

    void setRed(uint8_t r) { red = r; }
    void setGreen(uint8_t g) { green = g; }
    void setBlue(uint8_t b) { blue = b; }

private:
    uint8_t red, green, blue;
};

class IoTDevice
{
protected:
    std::string id;
    std::string name;
    bool state;

private:
    uint32_t _checksum;

public:
    IoTDevice(std::string id, std::string name) : id(id), name(name), state(false), _checksum(0) {}
    virtual ~IoTDevice() = default;
    virtual void describe(){};
    uint32_t checksum()
    {
        return _checksum;
    };
    void setChecksum(uint32_t val)
    {
        this->_checksum = val;
    };

    void setState(bool newState)
    {
        state = newState;
    }

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    bool getState() const { return state; }
    enum class supportedActions
    {
        setState = 1
    };
};

class Shutter : public IoTDevice
{
public:
    Shutter(std::string id, std::string name) : IoTDevice(id, name), height(0) {}

    int height;

    enum class supportedActions
    {
        setState = 1,
        setHeight
    };
};

class Light : public IoTDevice
{
public:
    Light(std::string id, std::string name) : IoTDevice(id, name) {}
};
class DimmLight : public Light
{
public:
    DimmLight(std::string id, std::string name) : Light(id, name), brightness(0) {}

    int brightness;
    enum class supportedActions
    {
        setState = 1,
        setBrightness
    };
};

class ColorLight : public DimmLight
{
public:
    ColorLight(std::string id, std::string name) : DimmLight(id, name), color(0, 0, 0)
    {
    }

    enum class supportedActions
    {
        setState = 1,
        setBrightness,
        setColor
    };
    RGB color;
};

class Media : public IoTDevice
{
public:
    Media(std::string id, std::string name) : IoTDevice(id, name) {}
};

class Climate : public IoTDevice
{
public:
    Climate(std::string id, std::string name) : IoTDevice(id, name) {}

    enum class supportedActions
    {
        setState = 0,
        setTemp
    };
};

class Sensor : public IoTDevice
{
public:
    Sensor(std::string id, std::string name) : IoTDevice(id, name), value(""), unit("")
    {
    }

    std::string value;
    std::string unit;
};

class IoTFactory
{
public:
    static IoTDevice *createIoTDev(const std::string &type, const std::string &name, std::string id)
    {
        if (type == "Light")
        {
            return new Light(id, name);
        }
        else if (type == "Climate")
        {
            return new Climate(id, name);
        }
        else if (type == "Sensor")
        {
            return new Sensor(id, name);
        }
        else if (type == "Shutter")
        {
            return new Shutter(id, name);
        }
        else if (type == "ColorLight")
        {
            return new ColorLight(id, name);
        }
        else if (type == "DimmLight")
        {
            return new DimmLight(id, name);
        }
        else if (type == "Media")
        {
            return new Media(id, name);
        }
        else
        {
            ESP_LOGE("IOTDEV", "Invalid DEV Type");

            return nullptr;
        }
    }
};

class IRepository
{
public:
    virtual void addDevice(IoTDevice *device) = 0;
    virtual void removeDevice(const std::string &id) = 0;
    virtual void updateDevice(IoTDevice *device) = 0;
};

class DeviceStore : public IRepository
{
private:
    std::map<std::string, std::unique_ptr<IoTDevice>> devices;
    PubSub<DeviceStatus> *statusPubSub;

public:
    DeviceStore(PubSub<DeviceStatus> *_statusPubSub = nullptr)
    {
        this->statusPubSub = _statusPubSub;
    }
    uint32_t getChecksum(const std::vector<std::string> &device_ids)
    {
        CRC32 crc;
        crc.reset();
        for (auto &dev : device_ids)
        {
            uint32_t chk = this->getDevice(dev)->checksum();
            crc.update(chk);
        }
        uint32_t checksum = crc.finalize();
        return checksum;
    }

    void addDevice(IoTDevice *device)
    {
        if (device)
        {
            devices[device->getId()] = std::unique_ptr<IoTDevice>(device);
        }
    }
    virtual void updateDevice(IoTDevice *device) override
    {
        if (device)
        {
            auto it = devices.find(device->getId());
            if (it != devices.end())
            {
                // Implementieren wenn genutzt
                /*   if (it->second.get() != device)
                  {
                      it->second = std::unique_ptr<IoTDevice>(device);
                  } */
            }
            else
            {
                devices[device->getId()] = std::unique_ptr<IoTDevice>(device);
            }
            notifyObservers(*device);
        }
    }
    void removeDevice(const std::string &id) override
    {
        auto it = devices.find(id);
        if (it != devices.end())
        {
            devices.erase(it);
        }
    }

    void initFromSettings(std::vector<device_t> devices)
    {
        for (auto const &dev : devices)
        {
            IoTDevice *iot_dev = IoTFactory::createIoTDev(dev.type, dev.name, dev.id);
            addDevice(iot_dev);
        }
    }

    std::vector<std::string> getDevices()
    {
        std::vector<std::string> keys;
        keys.reserve(this->devices.size());

        for (const auto &pair : this->devices)
        {
            keys.push_back(pair.first);
        }

        return keys;
    }

    IoTDevice *getDevice(std::string id)
    {
        auto it = devices.find(id);
        if (it != devices.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    bool setState(std::string id, bool state)
    {
        IoTDevice *dev = getDevice(id);
        return setState(dev, state);
    }

    bool setState(IoTDevice *dev, bool state)
    {
        if (dev != nullptr)
        {
            dev->setState(state);
            notifyObservers(*dev);
            return true;
        }
        return false;
    }

    void describeDevice(std::string id)
    {
        auto it = devices.find(id);
        if (it != devices.end())
        {
            it->second->describe();
        }
        else
        {
            std::cout << "Device not Found" << std::endl;
        }
    }

    void notifyObservers(const IoTDevice &device)
    {
        if (this->statusPubSub != nullptr)
        {
            DeviceStatus status;
            strcpy(status.device_id, device.getId().c_str());
            this->statusPubSub->publish(status);
        }
    }
};
