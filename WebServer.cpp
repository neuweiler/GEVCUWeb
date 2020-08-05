/*
 * WebServer.cpp
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


#include "WebServer.h"

WebServer webServer;

const char *WebServer::MIME_TYPE_JSON = "application/json";
fs::FS *WebServer::fileSystem = NULL;

WebServer::WebServer()
{
    uploadPath = "";
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

    setupAP();
    setupOTA();
    setupFilesystem();
    setupWebserver();
}

void WebServer::loop()
{
    ArduinoOTA.handle();
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
            logger.debug("SD_MMC Card Type: %s",
                    (cardType == CARD_MMC ? "MMC" : (cardType == CARD_SD ? "SDSC" : (cardType == CARD_SDHC ? "SDHC" : "unknown"))));
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

void WebServer::handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        logger.info("UploadStart: %s/%s", uploadPath.c_str(), filename.c_str());
        uploadFile = fileSystem->open(uploadPath + "/" + filename, FILE_WRITE);
    }
    uploadFile.write(data, len);
    if (final) {
        logger.info("UploadEnd: %s/%s (%d)", uploadPath.c_str(), filename.c_str(), index + len);
        uploadFile.close();
    }
}

void handleFileUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    webServer.handleUpload(request, filename, index, data, len, final);
}

String WebServer::fileList(String path)
{
    uploadPath = path;
    File root = fileSystem->open(path);
    String output = "<html><body><table>";
    if (path.length() > 1) {
        String parent = uploadPath.substring(0, uploadPath.lastIndexOf('/'));
        if (parent.length() == 0) {
            parent = "/";
        }
        output += "<tr><td colspan='4'><a href='/list?dir=" + parent + "'>..</a></td></tr>";
    }
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            time_t time = file.getLastWrite();
            String prefix = (file.isDirectory() ? "/list?dir=/" : "");
            const char *name = file.name() + 1;
            output += "<tr>";
            output += "<td><a href='" + prefix + name + "'>" + name + "</a></td>";
            output += "<td style='text-align: right;'>" + (file.isDirectory() ? "(dir)" : String(file.size())) + "</td>";
            output += "<td style='text-align: right;'>" + String(ctime(&time)) + "</td>";
            output += "<td><a href='/delete?file=/" + (String) name + "'>delete</a></td>";
            output += "</tr>";
            file = root.openNextFile();
        }
    }
    output += "</table>";
    output += "<form action='/upload' method='post' enctype='multipart/form-data'>";
    output += "Upload File: <input type='file' id='uploadFile' name='filename'>";
    output += "<input type='submit'>";
    output += "</form>";
    output += "</body></html>";

    return output;
}

void WebServer::deleteFile(String file)
{
    logger.info("delete: %s", file.c_str());
    if (!fileSystem->remove(file)) {
        fileSystem->rmdir(file);
    }
}

void WebServer::setupWebserver()
{
    server->on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(*fileSystem, "/config", "application/json", false, [](const String &key) {
                    return gevcuAdapter.getConfigParameter(key);
                });
    });

    server->on("/saveConfig", HTTP_POST | HTTP_GET, [](AsyncWebServerRequest *request) {
        size_t paramCount = request->params();
        for (int i = 0; i < paramCount; i++) {
            AsyncWebParameter* param = request->getParam(i);
            if (logger.isDebug()) {
                logger.debug("input param: %s = %s", param->name().c_str(), param->value());
            }
            gevcuAdapter.setConfigParameter(param->name(), param->value());
        }
        request->redirect("/settings/index.html");
    });

    server->on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", gevcuAdapter.getLog());
    });

    server->on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = (request->hasArg("dir") ? request->arg("dir") : "/");
        String list = webServer.fileList(path);
        request->send(200, "text/html", list);
    });

    server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->redirect("/list?dir=" + webServer.getUploadPath());
    }, handleFileUpload);
    server->onFileUpload(handleFileUpload);

    server->on("/delete", HTTP_GET, [](AsyncWebServerRequest *request) {
        String file = (request->hasArg("file") ? request->arg("file") : "");
        if (file.length() > 0) {
            webServer.deleteFile(file);
        }
        request->redirect("/list?dir=" + webServer.getUploadPath());
    });

    server->serveStatic("/", *fileSystem, "/").setDefaultFile("index.html");
    server->begin();

    MDNS.begin("gevcu");
    logger.info("HTTP server started, use http://gevcu.local");
}

AsyncWebServer* WebServer::getWebServer()
{
    return server;
}

String WebServer::getUploadPath()
{
    return this->uploadPath;
}

void WebServer::setupOTA()
{
    ArduinoOTA.onStart([]() {
        logger.info("Start updating %s", (ArduinoOTA.getCommand() == U_FLASH ? "flash" : "filesystem"));
    })
    .onEnd([]() {
        logger.info("Update finished");
    })
    .onError([](ota_error_t error) {
        logger.error("Update Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) logger.error("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) logger.error("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) logger.error("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) logger.error("Receive Failed");
        else if (error == OTA_END_ERROR) logger.error("End Failed");
    });
    ArduinoOTA.begin();
}
