/*
 * Webserver.h
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <ESPmDNS.h>
#include <SD_MMC.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Logger.h"
#include "Configuration.h"
#include "GevcuParams.h"

class WebServer
{
public:
    WebServer();
    virtual ~WebServer();
    void start(Configuration *configuration);
    AsyncWebServer *getWebServer();

private:
    void setupAP();
    void setupFilesystem();
    void setupWebserver();
    static String processor2(const String &key);

    Configuration *config;
    AsyncWebServer *server;
    static const char *MIME_TYPE_JSON;
    static fs::FS *fileSystem;
};

#endif /* WEBSERVER_H_ */
