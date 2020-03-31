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
    config = NULL;
}

GevcuAdapter::~GevcuAdapter()
{
}

void GevcuAdapter::start(Configuration *configuration)
{
    config = configuration;
    webSocket.subscribe(this);

    loadDummyData();
}

void GevcuAdapter::loadDummyData()
{
    gevcuConfig["system.systemType"] = "4";
    gevcuConfig["system.logLevel"] = "2";
    gevcuConfig["system.prechargeMillis"] = "1500";
    gevcuConfig["system.coolingTempOn"] = "50";
    gevcuConfig["system.coolingTempOff"] = "40";
    gevcuConfig["system.heaterTemperatureOn"] = "16";

    gevcuConfig["charger.maximumInputCurrent"] = "32";
    gevcuConfig["charger.constantCurrent"] = "32";
    gevcuConfig["charger.constantVoltage"] = "416.3";
    gevcuConfig["charger.terminateCurrent"] = "4";
    gevcuConfig["charger.minBatteryVoltage"] = "270";
    gevcuConfig["charger.maxBatteryVoltage"] = "420";
    gevcuConfig["charger.minimumTemperature"] = "5";
    gevcuConfig["charger.maximumTemperature"] = "60";
    gevcuConfig["charger.maximumAmpereHours"] = "100";
    gevcuConfig["charger.maximumChargeTime"] = "600";
    gevcuConfig["charger.deratingRate"] = "10";
    gevcuConfig["charger.deratingReferenceTmp"] = "50";
    gevcuConfig["charger.hystereseStopTemp"] = "60";
    gevcuConfig["charger.hystereseResumeTemp"] = "40";

    gevcuConfig["controls.positionRegenMaximum"] = "10";
    gevcuConfig["controls.positionRegenMinimum"] = "20";
    gevcuConfig["controls.positionForwardStart"] = "30";
    gevcuConfig["controls.positionHalfPower"] = "80";
    gevcuConfig["controls.minimumRegen"] = "5";
    gevcuConfig["controls.maximumRegen"] = "80";
    gevcuConfig["controls.creepLevel"] = "10";
    gevcuConfig["controls.creepSpeed"] = "700";
    gevcuConfig["controls.brakeMinimumRegen"] = "20";
    gevcuConfig["controls.brakeMaximumRegen"] = "70";
    gevcuConfig["controls.slewRate"] = "100";
    gevcuConfig["controls.brakeHold"] = "50";
    gevcuConfig["controls.brakeHoldForceCoef"] = "10";
    gevcuConfig["controls.reversePercent"] = "50";
    gevcuConfig["controls.gearChangeSupport"] = "true";
    gevcuConfig["controls.cruiseKp"] = "0.5";
    gevcuConfig["controls.cruiseKi"] = "0.2";
    gevcuConfig["controls.cruiseKd"] = "0.02";
    gevcuConfig["controls.cruiseLongPressDelta"] = "500";
    gevcuConfig["controls.cruiseStepDelta"] = "300";
    gevcuConfig["controls.cruiseUseRpm"] = "true";
    gevcuConfig["controls.cruiseSpeedStep"] = "100";
    gevcuConfig["controls.cruiseSpeedSet"] = "0";

    gevcuConfig["dashboard.torqueRange"] = "-220,220";
    gevcuConfig["dashboard.rpmRange"] = "0,8000";
    gevcuConfig["dashboard.currentRange"] = "-200,200";
    gevcuConfig["dashboard.motorTempRange"] = "0,90,120";
    gevcuConfig["dashboard.controllerTempRange"] = "0,60,80";
    gevcuConfig["dashboard.batteryRangeLow"] = "297,357,368";
    gevcuConfig["dashboard.batteryRangeHigh"] = "387,405,418";
    gevcuConfig["dashboard.socRange"] = "0,20,100";
    gevcuConfig["dashboard.powerRange"] = "-150,150";
    gevcuConfig["dashboard.chargeInputLevels"] = "0,56";
    gevcuConfig["dashboard.cruiseUseRpm"] = "true";
    gevcuConfig["dashboard.cruiseSpeedStep"] = "100";
    gevcuConfig["dashboard.cruiseSpeedSet"] = "1000";

    gevcuConfig["dcdc.mode"] = "0";
    gevcuConfig["dcdc.lowVoltageCommand"] = "13.5";
    gevcuConfig["dcdc.hvUndervoltageLimit"] = "270";
    gevcuConfig["dcdc.lvBuckCurrentLimit"] = "150";
    gevcuConfig["dcdc.hvBuckCurrentLimit"] = "20";
    gevcuConfig["dcdc.highVoltageCommand"] = "400";
    gevcuConfig["dcdc.lvUndervoltageLimit"] = "11.5";
    gevcuConfig["dcdc.lvBoostCurrentLimit"] = "150";
    gevcuConfig["dcdc.hvBoostCurrentLimit"] = "20";

    gevcuConfig["devices.x1000"] = "0";
    gevcuConfig["devices.x1001"] = "1";
    gevcuConfig["devices.x1002"] = "0";
    gevcuConfig["devices.x1010"] = "1";
    gevcuConfig["devices.x1020"] = "0";
    gevcuConfig["devices.x1022"] = "0";
    gevcuConfig["devices.x1031"] = "0";
    gevcuConfig["devices.x1032"] = "0";
    gevcuConfig["devices.x1033"] = "1";
    gevcuConfig["devices.x1034"] = "1";
    gevcuConfig["devices.x1040"] = "1";
    gevcuConfig["devices.x2000"] = "0";
    gevcuConfig["devices.x2001"] = "1";
    gevcuConfig["devices.x3000"] = "1";
    gevcuConfig["devices.x5001"] = "0";
    gevcuConfig["devices.x5003"] = "1";
    gevcuConfig["devices.x6000"] = "0";
    gevcuConfig["devices.x6500"] = "0";

    gevcuConfig["inputs.abs"] = "255";
    gevcuConfig["inputs.enable"] = "0";
    gevcuConfig["inputs.reverse"] = "2";
    gevcuConfig["inputs.chargePwrAvail"] = "1";
    gevcuConfig["inputs.interlock"] = "255";
    gevcuConfig["inputs.gearChange"] = "3";

    gevcuConfig["motor.numberPotMeters"] = "2";
    gevcuConfig["motor.throttleSubType"] = "1";
    gevcuConfig["motor.aDcPin1"] = "11";
    gevcuConfig["motor.aDcPin2"] = "12";
    gevcuConfig["motor.minimumLevel"] = "111";
    gevcuConfig["motor.minimumLevel2"] = "222";
    gevcuConfig["motor.maximumLevel"] = "3333";
    gevcuConfig["motor.maximumLevel2"] = "4444";
    gevcuConfig["motor.brakeMinimumLevel"] = "11";
    gevcuConfig["motor.brakeMaximumLevel"] = "4001";
    gevcuConfig["motor.carType"] = "0";
    gevcuConfig["motor.speedMax"] = "7000";
    gevcuConfig["motor.torqueMax"] = "220";
    gevcuConfig["motor.motorMode"] = "1";
    gevcuConfig["motor.nominalVolt"] = "399";
    gevcuConfig["motor.invertDirection"] = "1";
    gevcuConfig["motor.maxMechPowerMotor"] = "110";
    gevcuConfig["motor.maxMechPowerRegen"] = "109";
    gevcuConfig["motor.dcVoltLimitMotor"] = "300";
    gevcuConfig["motor.dcVoltLimitRegen"] = "410";
    gevcuConfig["motor.dcCurrentLimitMotor"] = "300";
    gevcuConfig["motor.dcCurrentLimitRegen"] = "200";
    gevcuConfig["motor.enableOscLimiter"] = "1";

    gevcuConfig["outputs.prechargeRelay"] = "1";
    gevcuConfig["outputs.mainContactor"] = "0";
    gevcuConfig["outputs.secondaryCont"] = "2";
    gevcuConfig["outputs.fastChargeCont"] = "3";
    gevcuConfig["outputs.enableMotor"] = "4";
    gevcuConfig["outputs.enableCharger"] = "5";
    gevcuConfig["outputs.enableDcDc"] = "6";
    gevcuConfig["outputs.enableHeater"] = "7";
    gevcuConfig["outputs.heaterValve"] = "255";
    gevcuConfig["outputs.heaterPump"] = "255";
    gevcuConfig["outputs.coolingPump"] = "255";
    gevcuConfig["outputs.coolingFan"] = "255";
    gevcuConfig["outputs.brakeLight"] = "255";
    gevcuConfig["outputs.reverseLight"] = "255";
    gevcuConfig["outputs.powerSteering"] = "255";
    gevcuConfig["outputs.warning"] = "255";
    gevcuConfig["outputs.powerLimit"] = "255";
    gevcuConfig["outputs.stateOfCharge"] = "255";
    gevcuConfig["outputs.statusLight"] = "255";
}

String GevcuAdapter::getConfigParameter(String key)
{
    return gevcuConfig[key];
}

//char buffer[1024];
//int pos = 0;

//HardwareSerial Serial1(1);

//    Serial1.begin(115200, SERIAL_8N1, PIN_SERIAL1_RX, PIN_SERIAL1_TX);


void GevcuAdapter::loop()
{
    sendUpdate();

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

void GevcuAdapter::event(String message) {
    logger.info("got event %s", message);
}


uint8_t i = 0;

void GevcuAdapter::sendUpdate()
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
    webSocket.send(data);

    i++;
    if (i > 100)
        i = 0;
}

