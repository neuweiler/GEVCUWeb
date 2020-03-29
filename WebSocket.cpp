/*
 * WebSocket.cpp
 *
 *  Created on: 17 Feb 2020
 *      Author: michaeln
 */

#include "WebSocket.h"

WebSocket::WebSocket()
{
    config = NULL;
    connected = false;
    webSocketHandler = new AsyncWebSocket("/ws");
}

WebSocket::~WebSocket()
{
    if (webSocketHandler) {
        free(webSocketHandler);
        webSocketHandler = NULL;
    }
}

extern WebSocket webSocket;

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    webSocket.onWebsocketEvent(server, client, type, arg, data, len);
}

void WebSocket::start(Configuration *configuration, AsyncWebServer *server)
{
    config = configuration;
    connected = false;
    webSocketHandler->onEvent(onEvent);
    server->addHandler(webSocketHandler);
    logger.info("WebSocket configured");
}

void WebSocket::onWebsocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT) {
        connected = true;
        logger.info("ws[%s][%u] connect", server->url(), client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        connected = false;
        logger.info("ws[%s][%u] disconnect", server->url(), client->id());
    } else if (type == WS_EVT_ERROR) {
        logger.warn("ws[%s][%u] error(%u): %s", server->url(), client->id(), *((uint16_t*) arg), (char*) data);
    } else if (type == WS_EVT_PONG) {
        logger.debug("ws[%s][%u] pong[%u]: %s", server->url(), client->id(), len, (len) ? (char*) data : "");
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo * info = (AwsFrameInfo*) arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len) {
            //the whole message is in a single frame and we got all of it's data
            if (logger.isDebug()) {
                logger.debug("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
            }

            if (info->opcode == WS_TEXT) {
                for (size_t i = 0; i < info->len; i++) {
                    msg += (char) data[i];
                }
            } else {
                char buff[3];
                for (size_t i = 0; i < info->len; i++) {
                    sprintf(buff, "%02x ", (uint8_t) data[i]);
                    msg += buff;
                }
            }
            // TODO
            logger.debug("%s", msg.c_str());
        }
    }
}

uint8_t i = 0;

void WebSocket::sendUpdate()
{
    doc.clear();
    doc["timeRunning"] = millis() / 1000;
    doc["systemState"] = (i / 25 + 5);
    doc["chargeLevel"] = i;
    doc["chargeHoursRemain"] = (10 - i / 10);
    doc["chargeMinsRemain"] = (59 - i * 0.6f);
    doc["maximumInputCurrent"] = i / 4.0f;
    doc["maximumInputCurrentLevel"] = i / 4.0f;
    doc["bitfieldIO"] = rand() % 100;
    doc["bitfieldMotor"] = rand() % 100;
    doc["bitfieldBms"] = rand() % 100;
    doc["enableRegen"] = ((i / 10) % 2 == 0 ? "true" : "false");
    doc["enableHeater"] = ((i / 20) % 2 == 0 ? "true" : "false");
    doc["enableCreep"] = ((i / 5) % 2 == 0 ? "true" : "false");
    doc["powerSteering"] = (((i + 3) / 10) % 2 == 0 ? "true" : "false");
    doc["throttle"] = rand() % 200 - 100;
    //    doc["brake"] = 0;
    //    doc["gear"] = 0;
    //    doc["torqueRequested"] = i;
    doc["torqueActual"] = rand() % 440 - 220;
    doc["speedActual"] = i * 90;
    doc["dcVoltage"] = (i * 2.2f + 220);
    doc["dcCurrent"] = (i * 5.5f - 275);
    //    doc["mechanicalPower"] = (i * 2.5f - 125);
    doc["temperatureMotor"] = i * 1.5f;
    doc["temperatureController"] = i;
    doc["dcDcHvVoltage"] = (i * 2.2f + 221);
    doc["dcDcHvCurrent"] = (i / 25);
    doc["dcDcLvVoltage"] = (i / 25.0f + 10);
    doc["dcDcLvCurrent"] = (i * 2);
    doc["dcDcTemperature"] = (i / 2.0f + 20);
    doc["chargerInputVoltage"] = (i * 0.8f + 180);
    doc["chargerInputCurrent"] = (i * 0.4);
    doc["chargerBatteryVoltage"] = (i * 2.3f + 240);
    doc["chargerBatteryCurrent"] = (i * 0.2f);
    doc["chargerTemperature"] = (i / 2.0f + 20);
    doc["temperatureCoolant"] = i;
    doc["temperatureHeater"] = i;
    doc["heaterPower"] = i * 60;
    doc["flowCoolant"] = (i / 25.0f + 10);
    doc["flowHeater"] = (i / 25.0f + 10);
    doc["temperatureBattery1"] = i;
    doc["temperatureBattery2"] = i;
    doc["temperatureBattery3"] = i;
    doc["temperatureBattery4"] = i;
    doc["temperatureBattery5"] = i;
    doc["temperatureBattery6"] = i;
    doc["temperatureExterior"] = i;
    doc["packVoltage"] = i;
    doc["packCurrent"] = i;
    doc["soc"] = (100 - i);
    doc["dischargeLimit"] = i;
    doc["chargeLimit"] = i;
    doc["chargeAllowed"] = ((i / 10) % 2 == 0 ? "true" : "false");
    doc["dischargeAllowed"] = ((i / 10) % 2 == 1 ? "true" : "false");
    doc["lowestCellTemp"] = i;
    doc["highestCellTemp"] = i;
    doc["lowestCellVolts"] = i / 30.0f;
    doc["highestCellVolts"] = (100 - i) / 30.0f;
    doc["averageCellVolts"] = i;
    doc["deltaCellVolts"] = i;
    doc["lowestCellResistance"] = i;
    doc["highestCellResistance"] = i;
    doc["averageCellResistance"] = i;
    doc["deltaCellResistance"] = i;
    doc["lowestCellTempId"] = i;
    doc["highestCellTempId"] = i;
    doc["lowestCellVoltsId"] = i;
    doc["highestCellVoltsId"] = i;
    doc["lowestCellResistanceId"] = i;
    doc["highestCellResistanceId"] = i;

    JsonObject limits = doc.createNestedObject("limits");
    JsonObject dcCurrent = limits.createNestedObject("dcCurrent");
    dcCurrent["min"] = (-240 + i);
    dcCurrent["max"] = (240 - i);
    JsonObject dcVoltage = limits.createNestedObject("dcVoltage");
    dcVoltage["min"] = (270 + i);
    dcVoltage["max"] = (450 - i);
    JsonObject temperatureMotor = limits.createNestedObject("temperatureMotor");
    temperatureMotor["min"] = (150 - i);
    temperatureMotor["max"] = (100 - i);

    String data;
    serializeJson(doc, data);
    webSocketHandler->textAll(data);

    i++;
    if (i > 100)
        i = 0;
}

void WebSocket::loop()
{
    sendUpdate();
    webSocketHandler->cleanupClients();
}
