/*
 * WLAN.cpp
 *
 * This class handles the connection to a consumer's WLAN and sends
 * the maximum available power to it. The purpose is to keep it's
 * consumption at or below what the PV panels are able to deliver.
 * So no power from a connected battery or grid is used.
 * This is useful e.g. when charging an electric car.
 *
 * When a client is connected to our AP, the output D5 goes high,
 * allowing an LED to signal a connected client. When this device is
 * connected to the consumer's WLAN, D6 will go high.
 *
 *  Created on: 4 Aug 2019
 *      Author: Michael Neuweiler
 */

#include "WLAN.h"

/**
 * Constructor
 */
WLAN::WLAN() {
	timestamp = 0;
	lastConnectionAttempt = 0;
	apConnected = false;
	stationConnected = false;
}

WLAN::~WLAN() {
}

/**
 * Initialize the WiFi client and output ports.
 */
void WLAN::init() {
	WiFi.persistent(false); // prevent flash memory wear ! (https://github.com/esp8266/Arduino/issues/1054)
	WiFi.mode(WIFI_AP_STA); // set as AP and as client
	WiFi.setAutoReconnect(false); // auto-reconnect tries every 1sec, messes up soft-ap (can't connect)
	WiFi.begin(config.wifiSsidRemote, config.wifiPasswordRemote);
	logger.info("started WiFi Station for SSID %s", config.wifiSsidRemote);

	// setup own AccessPoint
	WiFi.softAP(config.wifiSsid, config.wifiPassword);
	delay(100); // wait for SYSTEM_EVENT_AP_START

	IPAddress localIp;
	localIp.fromString(config.wifiAddress);
	IPAddress gateway;
	gateway.fromString(config.wifiGateway);
	IPAddress subnet;
	subnet.fromString(config.wifiNetmask);
	WiFi.softAPConfig(localIp, gateway, subnet);
	logger.info("started WiFi AP %s on ip %s", config.wifiSsid, WiFi.softAPIP().toString().c_str());

	http.setReuse(true);

	setupOTA();
}

/**
 * The main processing logic.
 */
void WLAN::loop() {
	checkConnection();
	if (stationConnected && (timestamp + config.currentUpdateInterval < millis())) {
		updateMaxCurrent();
		timestamp = millis();
	}
	ArduinoOTA.handle();

}

/**
 * Check if a connection to or from a device is established.
 *  Tries to (re)establish a connection to the target WLAN.
 *
 * As in dual mode (WIFI_AP_STA) the auto reconnect has to be
 * disabled, we need to manually try to connect to the AP every 15sec.
 */
void WLAN::checkConnection() {
	if (WiFi.isConnected()) {
		stationConnected = true;
	} else {
		stationConnected = false;
		// we try to (re)establish connection every 15sec, this allows softAP to work (although it gets blocked for 1-2sec)
		if (millis() - lastConnectionAttempt > config.wifiReconnectInterval) {
			logger.info("attempting to (re)connect to %s", config.wifiSsidRemote);
			WiFi.begin(config.wifiSsidRemote, config.wifiPasswordRemote);
			lastConnectionAttempt = millis();
		}
	}
	apConnected = WiFi.softAPgetStationNum() > 0;
}

/**
 * Retrieve the available max current for charging from a remote device, e.g. a solar inverter
 */
void WLAN::updateMaxCurrent() {
/*	WiFiClient client;
	if (client.connect(config.currentUpdateHost, config.currentUpdatePort, 2000)) {
		client.println("GET /maxCurrent HTTP/1.0");
//		client.println("Host: arduinojson.org"));
		client.println("Connection: close");
		client.println();

		char status[32] = { 0 };
		client.readBytesUntil('\r', status, sizeof(status));
		if (strcmp(status + 9, "200 OK") == 0) {
			if (client.find("\r\n\r\n")) {
				const size_t capacity = JSON_OBJECT_SIZE(1) + 60;
				DynamicJsonDocument doc(capacity);
				DeserializationError error = deserializeJson(doc, client);
				if (error) {
					logger.error("deserialize: %s", error.f_str());
				} else {
					logger.info("max current: %d", doc["maxCurrent"].as<long>());
				}
			}
		}
		client.stop();

	}
*/
	http.begin(config.currentUpdateHost, config.currentUpdatePort, config.currentUpdateUri);
	int httpCode = http.GET();
	if (httpCode > 0) {
		logger.debug("HTTP GET: %d", httpCode);
		if (httpCode == HTTP_CODE_OK) {
			DeserializationError error = deserializeJson(doc, http.getStream());
			if (error) {
				logger.error("deserialize: %s", error.f_str());
			} else {
				logger.info("max current: %d", doc["maxCurrent"].as<long>());
				gevcuAdapter.setConfigParameter("overrideInputCurrent", doc["maxCurrent"].as<String>());
			}

//			String response = http.getString();
//			logger.info("http get: %s", response.c_str());
		}
	} else {
		logger.error("HTTP GET: %s", http.errorToString(httpCode).c_str());
	}
	http.end();
}

void WLAN::setupOTA() {
	ArduinoOTA.setHostname("gevcu");

	ArduinoOTA.onStart([]() {
		logger.info("Start updating %s", (ArduinoOTA.getCommand() == U_FLASH ? "flash" : "filesystem"));
		gevcuAdapter.stopHeartBeat();
	});
	ArduinoOTA.onEnd([]() {
		logger.info("Update finished");
		gevcuAdapter.startHeartBeat();
	});
	ArduinoOTA.onError([](ota_error_t error) {
		logger.error("Update Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR)
			logger.error("Auth Failed");
		else if (error == OTA_BEGIN_ERROR)
			logger.error("Begin Failed");
		else if (error == OTA_CONNECT_ERROR)
			logger.error("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR)
			logger.error("Receive Failed");
		else if (error == OTA_END_ERROR)
			logger.error("End Failed");
	});

	ArduinoOTA.begin();
	logger.info("OTA initialized");
}

WLAN wlan;
