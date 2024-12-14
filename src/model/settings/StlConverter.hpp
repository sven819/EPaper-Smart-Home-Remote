// Quelle: https://arduinojson.org/v6/how-to/create-converters-for-stl-containers/
#pragma once

#include <map>
#include <vector>
#include <ArduinoJson.h>

namespace ArduinoJson
{
  template <typename T>
  struct Converter<std::map<std::string, T>>
  {
    static void toJson(const std::map<std::string, T> &src, JsonVariant dst)
    {
      JsonObject obj = dst.to<JsonObject>();
      for (const auto &item : src)
        obj[item.first] = item.second;
    }

    static std::map<std::string, T> fromJson(JsonVariantConst src)
    {
      std::map<std::string, T> dst;
      for (JsonPairConst item : src.as<JsonObjectConst>())
        dst[item.key().c_str()] = item.value().as<T>();
      return dst;
    }

    static bool checkJson(JsonVariantConst src)
    {
      JsonObjectConst obj = src;
      bool result = obj;
      for (JsonPairConst item : obj)
        result &= item.value().is<T>();
      return result;
    }
  };
}

namespace ArduinoJson
{
  template <typename T>
  struct Converter<std::vector<T>>
  {
    static void toJson(const std::vector<T> &src, JsonVariant dst)
    {
      JsonArray array = dst.to<JsonArray>();
      for (T item : src)
        array.add(item);
    }

    static std::vector<T> fromJson(JsonVariantConst src)
    {
      std::vector<T> dst;
      for (T item : src.as<JsonArrayConst>())
        dst.push_back(item);
      return dst;
    }

    static bool checkJson(JsonVariantConst src)
    {
      JsonArrayConst array = src;
      bool result = array;
      for (JsonVariantConst item : array)
        result &= item.is<T>();
      return result;
    }
  };
}
