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
    binaryDataCount = 0;
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
    Serial2.setRxBufferSize(4096);

    logger.info("GevcuAdapter configured");
}

String GevcuAdapter::getConfigParameter(String key)
{
    return gevcuConfig[key];
}

void GevcuAdapter::setConfigParameter(String key, String value)
{
    gevcuConfig[key] = value;
    Serial2.printf("cfg:%s=%s\r\n", key.c_str(), value.c_str());
}

void GevcuAdapter::loop()
{
    while (Serial2.available()) {
        int ch = Serial2.read();
//Serial.printf("%c (%X)\n", ch, ch);
        if (binaryDataCount > 0) { // expecting binary data
            serialBuffer[bufPos++] = ch;
            if (serialBuffer[0] == 0xaa) {
                if (bufPos > 2 && processBinaryData(serialBuffer[1], bufPos - 2, &serialBuffer[2])) {
                    bufPos = 0;
                    if (--binaryDataCount == 0) {
                        String data;
                        serializeJson(doc, data);
logger.info("sending to websocket: %s", data.c_str());
                        webSocket.send(data);
                    }
                }
            } else { // expecting binary data but got no valid lead char (0xaa) -> drop this entry
                logger.warn("dropping invalid data frame");
                bufPos = 0;
                binaryDataCount--;
            }
        } else if (ch == 13 || ch == 10 || ch == 0 || bufPos > 1022) { // process config/log data
            serialBuffer[bufPos] = 0;
            if (bufPos > 4) { // drop strings smaller than 4 characters
                processInput(serialBuffer);
            }
            bufPos = 0;
        } else if (ch != -1) {
            serialBuffer[bufPos++] = ch;
        }
    }
}

bool GevcuAdapter::processBinaryData(char code, uint16_t len, char *data)
{
    DataPoint *dp = findDataPoint(code);
    if (dp && len < dp->length) {
        return false;
    }

    if (dp->length == 1 && dp->unsignd) {
        doc[dp->label] = (uint8_t)data[0];
    } else if (dp->length == 2 && dp->unsignd) {
        doc[dp->label] = (dp->divisor == 0 ? readUInt16(data) : readUInt16(data) / (double) dp->divisor);
    } else if (dp->length == 2 && !dp->unsignd) {
        doc[dp->label] = (dp->divisor == 0 ? readInt16(data) : readInt16(data) / (double) dp->divisor);
    } else if (dp->length == 4 && dp->unsignd) {
        doc[dp->label] = serialized((dp->divisor == 0 ? String(readUInt32(data)) : String(readUInt32(data) / (double) dp->divisor)));
    } else {
        logger.warn("unable to process data: code %d, length %d", code, len);
    }
    return true;
}

uint32_t GevcuAdapter::readUInt32(char *data) {
    return (uint32_t) ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
}

int16_t GevcuAdapter::readInt16(char *data) {
    return (int16_t) ((data[0] << 8) | data[1]);
}

uint16_t GevcuAdapter::readUInt16(char *data) {
    return (uint16_t) ((data[0] << 8) | data[1]);
}

void GevcuAdapter::processInput(String input)
{
    logger.info("input from GEVCU: %s", input.c_str());

    if (input.startsWith("data:")) {
        binaryDataCount = input.substring(5).toInt();
        doc.clear();
    } else if (input.startsWith("cfg:")) {
        int pos = input.indexOf('=');
        if (pos > 0) {
            String key = input.substring(4, pos);
            String value = input.substring(pos + 1);
            gevcuConfig[key] = value;
        }
    } else if (input.startsWith("json:")) {
        webSocket.send(input.substring(5));
    } else {
        logger.warn("unknown data from GEVCU: %s", input.c_str());
    }
}

void GevcuAdapter::event(String message)
{
    logger.info("sending event '%s' to GEVCU", message.c_str());
    Serial2.println("cmd:" + message);
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
