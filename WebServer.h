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
#include "GevcuAdapter.h"

class WebServer
{
public:
    WebServer();
    virtual ~WebServer();
    void start(Configuration *configuration);
    AsyncWebServer *getWebServer();
    void deleteFile(String file);
    void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
    String getUploadPath();

private:
    void setupAP();
    void setupFilesystem();
    void setupWebserver();
    String fileList(String path);

    Configuration *config;
    AsyncWebServer *server;
    static const char *MIME_TYPE_JSON;
    static fs::FS *fileSystem;
    File uploadFile;
    String uploadPath;
};

extern WebServer webServer;

#endif /* WEBSERVER_H_ */
