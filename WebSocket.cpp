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

void append(String &data, String label, int16_t value)
{
    data += "\"";
    data += label;
    data += "\": ";
    data += value;
    data += ",\r";
}

void append(String &data, String label, String value)
{
    data += "\"";
    data += label;
    data += "\": \"";
    data += value;
    data += "\",\r";
}

void appendf(String &data, String label, float value)
{
    data += "\"";
    data += label;
    data += "\": ";
    data += value;
    data += ",\r";
}

uint8_t i = 0;

void WebSocket::sendUpdate()
{
    String data = String("{");
    append(data, "timeRunning", millis() / 1000);
    append(data, "systemState", 8);
    //    append(data, "systemState", (i < 90 ? (i / 10 + 1) : 99));
    append(data, "chargeLevel", i);
    append(data, "chargeHoursRemain", (10 - i / 10));
    appendf(data, "chargeMinsRemain", (59 - i * 0.6f));
    append(data, "systemState", (i < 90 ? (i / 10 + 1) : 99));
    append(data, "bitfield1", rand() % 100);
    append(data, "bitfield2", rand() % 100);
    append(data, "bitfield3", rand() % 100);
    append(data, "enableRegen", ((i / 10) % 2 == 0 ? "true" : "false"));
    append(data, "enableHeater", ((i / 20) % 2 == 0 ? "true" : "false"));
    append(data, "enableCreep", ((i / 5) % 2 == 0 ? "true" : "false"));
    append(data, "powerSteering", (((i + 3) / 10) % 2 == 0 ? "true" : "false"));
    append(data, "throttle", rand() % 200 - 100);
    //    append(data, "brake", 0);
    //    append(data, "gear", 0);
    //    append(data, "torqueRequested", i);
    appendf(data, "torqueActual", rand() % 440 - 220);
    append(data, "speedActual", i * 90);
    appendf(data, "dcVoltage", (i * 2.2f + 220));
    appendf(data, "dcCurrent", (i * 5.5f - 275));
    //    appendf(data, "mechanicalPower", (i * 2.5f - 125));
    appendf(data, "temperatureMotor", i * 1.5f);
    append(data, "temperatureController", i);
    appendf(data, "dcDcHvVoltage", (i * 2.2f + 221));
    append(data, "dcDcHvCurrent", (i / 25));
    appendf(data, "dcDcLvVoltage", (i / 25.0f + 10));
    append(data, "dcDcLvCurrent", (i * 2));
    appendf(data, "dcDcTemperature", (i / 2.0f + 20));
    appendf(data, "chargerInputVoltage", (i * 0.8f + 180));
    append(data, "chargerInputCurrent", (i * 0.4));
    appendf(data, "chargerBatteryVoltage", (i * 2.3f + 240));
    appendf(data, "chargerBatteryCurrent", (i * 0.2f));
    appendf(data, "chargerTemperature", (i / 2.0f + 20));
    append(data, "temperatureCoolant", i);
    append(data, "temperatureHeater", i);
    append(data, "heaterPower", i * 60);
    appendf(data, "flowCoolant", (i / 25.0f + 10));
    appendf(data, "flowHeater", (i / 25.0f + 10));
    append(data, "temperatureBattery1", i);
    append(data, "temperatureBattery2", i);
    append(data, "temperatureBattery3", i);
    append(data, "temperatureBattery4", i);
    append(data, "temperatureBattery5", i);
    append(data, "temperatureBattery6", i);
    append(data, "temperatureExterior", i);
    append(data, "packVoltage", i);
    append(data, "packCurrent", i);
    append(data, "soc", (100 - i));
    append(data, "dischargeLimit", i);
    append(data, "chargeLimit", i);
    append(data, "chargeAllowed", ((i / 10) % 2 == 0 ? "true" : "false"));
    append(data, "dischargeAllowed", ((i / 10) % 2 == 1 ? "true" : "false"));
    append(data, "lowestCellTemp", i);
    append(data, "highestCellTemp", i);
    appendf(data, "lowestCellVolts", i / 30.0f);
    appendf(data, "highestCellVolts", (100 - i) / 30.0f);
    append(data, "averageCellVolts", i);
    append(data, "deltaCellVolts", i);
    append(data, "lowestCellResistance", i);
    append(data, "highestCellResistance", i);
    append(data, "averageCellResistance", i);
    append(data, "deltaCellResistance", i);
    append(data, "lowestCellTempId", i);
    append(data, "highestCellTempId", i);
    append(data, "lowestCellVoltsId", i);
    append(data, "highestCellVoltsId", i);
    append(data, "lowestCellResistanceId", i);
    append(data, "highestCellResistanceId", i);

    data += "\"limits\": { \"dcCurrent\": { \"min\": ";
    data += (-240 + i);
    data += ",\"max\": ";
    data += (240 - i);
    data += "},\"dcVoltage\": { \"min\": ";
    data += (270 + i);
    data += ",\"max\": ";
    data += (450 - i);
    data += "},\"temperatureMotor\": { \"max\": ";
    data += (150 - i);
    data += "},\"temperatureController\": { \"max\": ";
    data += (100 - i);
    data += "}}";
    data += "}";

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
