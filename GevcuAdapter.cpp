/*
 * GevcuAdapter.cpp
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

#include "GevcuAdapter.h"

GevcuAdapter gevcuAdapter;

GevcuAdapter::GevcuAdapter()
{
    bufPos = 0;
    dataCount = 0;
    config = NULL;
}

GevcuAdapter::~GevcuAdapter()
{
}

void GevcuAdapter::start(Configuration *configuration)
{
    config = configuration;
    webSocket.subscribe(this);

    Serial2.begin(115200);

    logger.info("GevcuAdapter configured");
}

String GevcuAdapter::getConfigParameter(String key)
{
    return gevcuConfig[key];
}

void GevcuAdapter::loop()
{
    while (Serial2.available()) {
        int ch = Serial2.read();
        // process live data
        if (bufPos > 2 && serialBuffer[0] == 0xff) {
            serialBuffer[bufPos++] = ch;
            if (processLiveData(serialBuffer[1], bufPos - 2, &serialBuffer[2])) {
                bufPos = 0;
                if (dataCount > 0) {
                    dataCount--;
                } else {
                    String data;
                    serializeJson(doc, data);
                    webSocket.send(data);
                }
            }
        // process config/log data
        } else if (bufPos > 4 && (ch == 13 || ch == 10 || ch == 0 || bufPos > 1022)) {
            serialBuffer[bufPos] = 0;
            processInput(serialBuffer);
            bufPos = 0;
        } else if (ch != -1) {
            serialBuffer[bufPos++] = ch;
        }
    }
}

bool GevcuAdapter::processLiveData(char code, uint16_t len, char *data)
{
    DataPoint *dp = findDataPoint(code);
    if (dp && dp->length == len) {
        if (dp->length == 1 && dp->unsignd) {
            doc[dp->label] = (uint8_t)data[0];
        } else if (dp->length == 2 && dp->unsignd) {
            doc[dp->label] = (dp->divisor == 0 ? readUInt16(data) : readUInt16(data) / (double) dp->divisor);
        } else if (dp->length == 2 && !dp->unsignd) {
            doc[dp->label] = (dp->divisor == 0 ? readInt16(data) : readInt16(data) / (double) dp->divisor);
        } else if (dp->length == 4 && !dp->unsignd) {
            doc[dp->label] = (dp->divisor == 0 ? readInt32(data)  : readInt32(data) / (double) dp->divisor);
        } else {
            logger.warn("unable to process data: code %d, length %d, unsigned %d", dp->code, dp->length, dp->unsignd);
        }
        return true;
    }
    logger.warn("could not find definition for dataPoint code %d", code);
    return false;
}

int32_t GevcuAdapter::readInt32(char *data) {
    return (int32_t) (data[3] | (data[2] << 8) | (data[1] << 16) | (data[0] << 24));
}

int16_t GevcuAdapter::readInt16(char *data) {
    return (int16_t) (data[1] | (data[0] << 8));
}

uint16_t GevcuAdapter::readUInt16(char *data) {
    return (uint16_t) (data[1] | (data[0] << 8));
}

void GevcuAdapter::processInput(char *input)
{
    logger.info("input from GEVCU: %s", input);

    if (strstr(input, "data:")) {
        dataCount = atol(input + 4);
        doc.clear();
    } else if (strstr(input, "cfg:")) {
        char *key = strtok(input, "=");
        char *value = strtok(NULL, "=");
        logger.info("config: %s = %s", key, value);
        gevcuConfig[key] = value;
    } else if (strstr(input, "log:")) {
        // TODO send log kmessage via WS
    }
}

void GevcuAdapter::event(String message)
{
    logger.info("sending event '%s' to GEVCU", message.c_str());
    Serial2.print(message);
}

DataPoint *GevcuAdapter::findDataPoint(char code)
{
    for (int i = 0; i < sizeof(dataPoints); i++) {
        if (dataPoints[i].code == code) {
            return &dataPoints[i];
        }
    }
    return NULL;
}

uint8_t i = 0;

void GevcuAdapter::sendDummyUpdate()
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
    webSocket.send(data);

    i++;
    if (i > 100)
        i = 0;
}
