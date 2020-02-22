/*
 * Webserver.cpp
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#include "WebServer.h"

const char *WebServer::MIME_TYPE_JSON = "application/json";
fs::FS *WebServer::fileSystem = NULL;

WebServer::WebServer()
{
    config = NULL;
    server = new AsyncWebServer(80);
}

WebServer::~WebServer()
{
    if (server) {
        free(server);
        server = NULL;
    }
}

void WebServer::start(Configuration *configuration)
{
    config = configuration;

    pinMode(config->PIN_WIFI_ENABLE, INPUT);
    pinMode(config->PIN_WIFI_RESET, INPUT);
    pinMode(config->PIN_WIFI_LED, OUTPUT);

    setupAP();
    setupFilesystem();
    setupWebserver();
}

void WebServer::setupAP()
{
    logger.info("Configuring access point: %s", config->SSID);

//    WiFi.persistent(false); // reduce web-server freezes
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config->SSID, config->PASSWORD);
    delay(100); // wait for SYSTEM_EVENT_AP_START

    IPAddress localIp(192, 168, 3, 10);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(localIp, localIp, subnet);

    logger.info("Access point started");
}

void WebServer::setupFilesystem()
{
    if (SD_MMC.begin()) {
        uint8_t cardType = SD_MMC.cardType();
        if (cardType != CARD_NONE) {
            logger.info("SD card mount successful");
            logger.debug("SD_MMC Card Type: %s", (cardType == CARD_MMC ? "MMC":
                    (cardType == CARD_SD ? "SDSC": (cardType == CARD_SDHC ? "SDHC" : "unknown"))));
            fileSystem = &SD_MMC;
        } else {
            logger.error("No SD_MMC card attached");
        }
        return;
    }
    // try SPIFFS as fall-back
    if (!SPIFFS.begin()) {
        logger.error("SPIFFS Mount failed");
    } else {
        logger.info("SPIFFS Mount successful");
        fileSystem = &SPIFFS;
    }
}

void WebServer::setupWebserver()
{
    server->on("/config/charger.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/charger.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/controls.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/controls.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/dashboard.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/dcdc.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/dcdc.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/devices.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/devices.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/inputs.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/inputs.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/motor.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/motor.js", MIME_TYPE_JSON, false, processor);
    });
    server->on("/config/outputs.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config/outputs.js", MIME_TYPE_JSON, false, processor);
    });
    server->serveStatic("/", *fileSystem, "/");
    server->begin();

    logger.info("HTTP server started: %s", WiFi.softAPIP().toString());
}

String WebServer::processor(const String& key)
{
    return (GevcuParams::getInstance()->findConfig(key));
}

AsyncWebServer *WebServer::getWebServer()
{
    return server;
}
