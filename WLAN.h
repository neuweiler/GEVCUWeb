/*
 * WLAN.h
 *
 *  Created on: 28 Apr 2021
 *      Author: Michael Neuweiler
 */

#ifndef WLAN_H_
#define WLAN_H_

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

#include "Logger.h"
#include "Configuration.h"
#include "GevcuAdapter.h"

class WLAN {
public:
	WLAN();
	virtual ~WLAN();
	void init();
	void loop();

private:
	void checkConnection();
	void updateMaxCurrent();
	void setupOTA();

	HTTPClient http;
	uint32_t timestamp;
	uint32_t lastConnectionAttempt;
	bool apConnected, stationConnected;
};

extern WLAN wlan;

#endif /* WLAN_H_ */
