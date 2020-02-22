/*
 * GevcuAdapter.cpp
 *
 *  Created on: 17 Feb 2020
 *      Author: michaeln
 */

#include "GevcuAdapter.h"

GevcuAdapter::GevcuAdapter()
{
    config = NULL;
}

GevcuAdapter::~GevcuAdapter()
{
}

void GevcuAdapter::start(Configuration *configuration)
{
    config = configuration;
}

//char buffer[1024];
//int pos = 0;

//HardwareSerial Serial1(1);

//    Serial1.begin(115200, SERIAL_8N1, PIN_SERIAL1_RX, PIN_SERIAL1_TX);


void GevcuAdapter::loop()
{
    /*    while (Serial1.available()) {
     int ch = Serial1.read();
     if ((ch == 13 || ch == 10 || ch == 0 || pos > 1023) && pos > 0) {
     buffer[pos] = 0;
     processInput(buffer);
     pos = 0;
     } else if (ch != -1) {
     buffer[pos++] = ch;
     }
     }
     */

}

void GevcuAdapter::processInput(char *input)
{
    /*    if (strstr(input, commandPrefix)) {
     input += 4;
     if (strstr(input, "SSND%:")) { // send to socket
     websocket.textAll(strchr(input + 10, ':') + 1);
     } else if (strchr(input, '=')) { // set parameter
     char *ch = strchr(input, '=');
     *ch = 0;
     char *value = ch + 2;
     ch = strrchr(value, '"');
     if (ch) {
     *ch = 0;
     }
     if (findConfig(String(input)) != NULL && configCounter < HASH_SIZE) {
     Serial.println("config: " + String(input) + "=" + String(value));
     config[configCounter++] = ConfigType(String(input), String(value));
     }
     } else if (strstr(input, "SRCV:") == input) { // request websocket data
     Serial.println("{ \"requestWsData\": \"" + String(input) + "\" }");
     } else if (strstr(input, "LSST:") == input) { // request active socket list
     Serial.println("{ \"activeSockets\": \"" + String(input) + "\" }");
     } else if (strstr(input, "LTCP:") == input) { // start websocket listener on specific port
     Serial.println("{ \"startWsListener\": \"" + String(input) + "\" }");
     } else if (strstr(input, "WNXT") == input) { // request next input parameter
     Serial.println("{ \"requestParam\": \"" + String(input) + "\" }");
     }
     //        Serial1.write(okString);
     //        Serial1.write('\r');
     } else {
     //        Serial1.write(errorString);
     //        Serial1.write('\r');
     }*/
}
