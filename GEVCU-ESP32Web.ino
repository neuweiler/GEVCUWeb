#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSockets.h>
#include <WebSocketsServer.h>
#include <FS.h>

const char *ssid = "ESP32ap";
const char *password = "12345678";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(2000);

uint32_t timestamp;
uint8_t i;

void setup()
{
    Serial.begin(115200);
    setupAP();
    setupFilesystem();
    setupWebserver();
    setupWebsocket();

    timestamp = millis();
    i = 0;
}

void setupAP()
{
    Serial.print("Configuring access point: ");
    Serial.println(ssid);

    IPAddress localIp(192, 168, 3, 10);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(localIp, localIp, subnet);
    WiFi.softAP(ssid, password);

    Serial.print("Access point started: ");
}

void setupFilesystem()
{
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS Mount failed");
    } else {
        Serial.println("SPIFFS Mount succesfull");
    }
}

void setupWebserver()
{
    server.on("/", handleRoot);
    server.begin();
    server.serveStatic("/", SPIFFS, "/");
    Serial.print("HTTP server started: ");
    Serial.println(WiFi.softAPIP());
}

void setupWebsocket()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started");
}

void handleRoot()
{
    server.send(200, "text/html", "<h1>You are connected</h1>");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
    switch (type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
    }
        break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);

        // send message to client
        webSocket.sendTXT(num, "message here");

        // send data to all connected clients
        webSocket.broadcastTXT("message here (broadcast)");
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    }
}

void loop()
{
    server.handleClient();
    webSocket.loop();

    if (timestamp + 1000 < millis()) {
        sendUpdate();
        timestamp = millis();
    }
}


void append(String data, String label, int16_t value)
{
    data += "\"";
    data += label;
    data += "\": ";
    data += value;
    data += ",\r";
}

void append(String data, String label, String value)
{
    data += "\"";
    data += label;
    data += "\": \"";
    data += value;
    data += "\",\r";
}

void appendf(String data, String label, float value)
{
    data += "\"";
    data += label;
    data += "\": ";
    data += value;
    data += ",\r";
}


void sendUpdate()
{
    String data = String("{}");
    append(data, "chargeLevel", i);
    append(data, "chargeHoursRemain", (10 - i / 10));
    appendf(data, "chargeMinsRemain", (59 - i * 0.6f));
    append(data, "timeRunning", millis() / 1000);
    append(data, "systemState", (i < 90 ? (i / 10 + 1) : 99));
    append(data, "bitfield1", (i + rand()) * 100);
    append(data, "bitfield2", (i + rand()) * 100);
    append(data, "bitfield3", (i + rand()) * 100);
    append(data, "enableRegen", ((i / 10) % 2 == 0 ? "true" : "false"));
    append(data, "enableHeater", ((i / 20) % 2 == 0 ? "true" : "false"));
    append(data, "enableCreep", ((i / 5) % 2 == 0 ? "true" : "false"));
    append(data, "powerSteering", (((i + 3) / 10) % 2 == 0 ? "true" : "false"));
    append(data, "throttle", (-100 + (rand() * 200)));
//    append(data, "brake", 0);
//    append(data, "gear", 0);
//    append(data, "torqueRequested", i);
    appendf(data, "torqueActual", (-220 + (rand() * 440)));
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
    data + ",\"max\": ";
    data += (450 - i);
    data += "},\"temperatureMotor\": { \"max\": ";
    data += (150 - i);
    data += "},\"temperatureController\": { \"max\": ";
    data += (100 - i);
    data += "}}";
    data += "}";

    webSocket.broadcastTXT(data);
    i++;
}

