/*
 * Configuration.h
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

class Configuration
{
public:
    Configuration();
    virtual ~Configuration();
    void load();

    const char *SSID;
    const char *PASSWORD;
    const char *commandPrefix;
    const char *okString;
    const char *errorString;

    uint8_t PIN_WIFI_ENABLE;
    uint8_t PIN_WIFI_RESET;
    uint8_t PIN_WIFI_LED;

    uint8_t PIN_SERIAL1_RX;
    uint8_t PIN_SERIAL1_TX;
};


#endif /* CONFIGURATION_H_ */
