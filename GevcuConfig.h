/*
 * GevcuParams.h
 *
 *  Created on: 16 Feb 2020
 *      Author: michaeln
 */

#ifndef GEVCUCONFIG_H_
#define GEVCUCONFIG_H_

#include <Arduino.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "Logger.h"

class ConfigType
{
public:
    String key;
    String value;
    ConfigType()
    {
    }
    ConfigType(String key, String value)
    {
        this->key = key;
        this->value = value;
    }
};

class GevcuConfig
{
public:
    virtual ~GevcuConfig();
    static GevcuConfig *getInstance();
    void toJson(JsonObject &root);
private:
    GevcuConfig();
    GevcuConfig(GevcuConfig const&); // copy disabled
    void operator=(GevcuConfig const&); // assigment disabled
};

#endif /* GEVCUCONFIG_H_ */
