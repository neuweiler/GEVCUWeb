/*
 * HeartBeat.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#include "HeartBeat.h"

HeartBeat::HeartBeat()
{
    config = NULL;
    led = false;
    connected = false;
    timestamp = millis();
}

HeartBeat::~HeartBeat()
{
}

void HeartBeat::start(Configuration *configuration)
{
    config = configuration;

    pinMode(config->PIN_WIFI_LED, OUTPUT);
}

void HeartBeat::loop()
{
    if (timestamp + 1000 < millis()) {
        led = !led;
        digitalWrite(config->PIN_WIFI_LED, led | connected);
        timestamp = millis();
    }
}
