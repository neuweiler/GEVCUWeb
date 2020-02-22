/*
 * Configuration.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#include "Configuration.h"

Configuration::Configuration()
{
    SSID = "GEVCU";
    PASSWORD = "verysecret";
    commandPrefix = "AT+i";
    okString = "I/OK";
    errorString = "I/ERROR";

    PIN_WIFI_ENABLE = 1; // D42 on GEVCU
    PIN_WIFI_RESET = 2; // D18 on GEVCU
    PIN_WIFI_LED = 3;

    PIN_SERIAL1_RX = 12;
    PIN_SERIAL1_TX = 13;
}

Configuration::~Configuration()
{
}

void Configuration::load() {

}
