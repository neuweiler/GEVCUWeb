/*
 * Configuration.cpp
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

#include "Configuration.h"

Configuration config;

void Configuration::load() {
	File file = fsHandler.getFS()->open("/config.json");
	DeserializationError error = deserializeJson(doc, file);
	if (error) {
		logger.error("could not parse config: %s", error.f_str());
	}
	wifiPinLed = doc["wifi"]["pinLed"] | 32;
	wifiHostname = doc["wifi"]["hostname"] | "gevcu";

	wifiStationSsid = doc["wifi"]["station"]["ssid"] | "";
	wifiStationPassword = doc["wifi"]["station"]["password"] | "";
	wifiStationReconnectInterval = doc["wifi"]["station"]["reconnectInterval"] | 15000;

	wifiApSsid = doc["wifi"]["ap"]["ssid"] | "GEVCU";
	wifiApPassword = doc["wifi"]["ap"]["password"] | "verysecret";
	wifiApChannel = doc["wifi"]["ap"]["channel"] | 13;
	wifiApAddress = doc["wifi"]["ap"]["address"] | "192.168.3.10";
	wifiApGateway = doc["wifi"]["ap"]["gateway"] | "192.168.3.10";
	wifiApNetmask = doc["wifi"]["ap"]["netmask"] | "255.255.255.0";

	currentUpdateInterval = doc["currentUpdate"]["interval"] | 5000;
	currentUpdateHost = doc["currentUpdate"]["host"] | "solar.local";
	currentUpdatePort = doc["currentUpdate"]["port"] | 80;
	currentUpdateUri = doc["currentUpdate"]["uri"] | "/maxCurrent";

	file.close();
}
