/*
 * HeartBeat.cpp
 *
 Copyright (c) 2020 Michael Neuweiler

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */


#include "HeartBeat.h"

HeartBeat heartBeat;

HeartBeat::HeartBeat()
{
    config = NULL;
    led = false;
    timestamp = millis();
    count = 0;
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
        digitalWrite(config->PIN_WIFI_LED, led | webSocket.isConnected());
        timestamp = millis();
        gevcuAdapter.sendHeartBeat(count++);
    }
}
