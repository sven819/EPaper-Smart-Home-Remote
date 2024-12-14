#pragma once

#include <string>
#include <map>
#include <variant>
#include "../model/settings/StlConverter.hpp"

class EntityAttribute
{
public:
    virtual void test(){};

    std::string name;
};
class EntityAttributeNum : public EntityAttribute
{
public:
    int value;
};
class EntityAttributeTxt : public EntityAttribute
{
public:
    std::string value;
};

class EntityState
{
public:
    EntityState() : state(""){};
    std::string id;
    std::string state;
    std::map<std::string, EntityAttribute *> attributes;
    long long lastChange;
    long long lastUpdate;
    bool boolState()
    {
        static const std::vector<std::string> values = {"on", "closed"};
        return std::find(values.begin(), values.end(), this->state) != values.end();
    }
    bool getIntAttribute(const std::string &key, int *target)
    {
        if (this->attributes.find(key) != this->attributes.end())
        {
            auto val = this->attributes.at(key);
            if (auto numAttr = dynamic_cast<EntityAttributeNum *>(val))
            {
                ESP_LOGI("HAModel", "Export INT; %d", numAttr->value);
                *target = numAttr->value;
                return true;
            }
            else if (auto txtAttr = dynamic_cast<EntityAttributeTxt *>(val))
            {
                try
                {
                    *target = std::stoi(txtAttr->value);
                }
                catch (std::invalid_argument const &ex)
                {
                    std::cout << "std::invalid_argument " << '\n';
                }
                catch (std::out_of_range const &ex)
                {
                    std::cout << "std::out_of_range " << '\n';
                }
            }
            else
            {
                std::cout << "Unbekannter Attributtyp";
                return false;
            }
        }
        return false;
    }
    bool getStringAttribute(const std::string &key, std::string *target)
    {
        if (this->attributes.find(key) != this->attributes.end())
        {
            auto val = this->attributes.at(key);
            if (auto numAttr = dynamic_cast<EntityAttributeNum *>(val))
            {
                *target = std::to_string(numAttr->value);
                return true;
            }
            else if (auto txtAttr = dynamic_cast<EntityAttributeTxt *>(val))
            {

                *target = txtAttr->value;
            }
            else
            {
                std::cout << "Unbekannter Attributtyp";
                return false;
            }
        }
        return false;
    }
    void print() const
    {
        std::cout << "State: " << state << "\n";
        std::cout << "Attributes:\n";
        for (const auto &[key, val] : attributes)
        {
            std::cout << "  " << key << ": ";

            if (auto numAttr = dynamic_cast<EntityAttributeNum *>(val))
            {
                std::cout << numAttr->value;
            }
            else if (auto txtAttr = dynamic_cast<EntityAttributeTxt *>(val))
            {
                std::cout << txtAttr->value;
            }
            else
            {
                std::cout << "Unbekannter Attributtyp";
            }

            std::cout << "\n";
        }
        std::cout << "Last Change: " << lastChange << "\n";
        std::cout << "Last Update: " << lastUpdate << "\n";
    }
};
namespace ArduinoJson
{
    template <>
    struct Converter<std::map<std::string, EntityAttribute *>>
    {
        static void toJson(const std::map<std::string, EntityAttribute *> &src, JsonVariant dst)
        {
        }

        static std::map<std::string, EntityAttribute *> fromJson(JsonVariantConst src)
        {
            std::map<std::string, EntityAttribute *> dst;
            for (JsonPairConst item : src.as<JsonObjectConst>())
            {
                std::string key = item.key().c_str();
                EntityAttribute *num;
                if (item.value().is<std::string>())
                {
                    num = new EntityAttributeTxt();
                    num->name = key;
                    ((EntityAttributeTxt *)num)->value = item.value().as<std::string>();
                }
                else if (item.value().is<int>())
                {
                    num = new EntityAttributeNum();
                    num->name = key;
                    ((EntityAttributeNum *)num)->value = item.value();
                }
                else
                    continue;

                dst[key] = num;
            }

            return dst;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}

namespace ArduinoJson
{
    template <>
    struct Converter<EntityState>
    {
        static bool toJson(const EntityState &src, JsonVariant dst)
        {
            return true;
        }

        static EntityState fromJson(JsonVariantConst src)
        {
            EntityState state;
            if (src.containsKey("s"))
                state.state = src["s"].as<std::string>();
            state.attributes = src["a"].as<std::map<std::string, EntityAttribute *>>();
            state.lastChange = src["lc"];
            state.lastUpdate = src["lu"];
            return state;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}

namespace ArduinoJson
{
    template <>
    struct Converter<std::vector<EntityState>>
    {
        static bool toJson(const std::vector<EntityState> &src, JsonVariant dst)
        {
            return true;
        }

        static std::vector<EntityState> fromJson(JsonVariantConst src)
        {
            std::vector<EntityState> states;
            if (src.containsKey("a"))
            {
                for (JsonPairConst additional : src["a"].as<JsonObjectConst>())
                {
                    EntityState state = additional.value().as<EntityState>();
                    state.id = additional.key().c_str();
                    states.push_back(state);
                }
            }
            if (src.containsKey("c"))
            {
                for (JsonPairConst change : src["c"].as<JsonObjectConst>())
                {
                    if (change.value().containsKey("+"))
                    {
                        EntityState state = change.value()["+"].as<EntityState>();
                        state.id = change.key().c_str();
                        states.push_back(state);
                    }
                }
            }
            return states;
        }

        static bool checkJson(JsonVariantConst src)
        {
            return true;
        }
    };
}
