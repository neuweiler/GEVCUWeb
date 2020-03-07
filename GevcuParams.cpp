/*
 * GevcuParams.cpp
 *
 *  Created on: 16 Feb 2020
 *      Author: michaeln
 */

#include "GevcuParams.h"

GevcuParams::GevcuParams()
{
}

GevcuParams::~GevcuParams()
{
}

/**
 * Return the instance of the singleton
 */
GevcuParams *GevcuParams::getInstance()
{
    static GevcuParams instance;
    return &instance;
}

void GevcuParams::getConfig(JsonObject &root) {
    root["logLevel"] = 2;

    JsonObject charger = root.createNestedObject("charger");
    charger["maximumInputCurrent"] = 32;
    charger["constantCurrent"] = 32;
    charger["constantVoltage"] = 416.3;
    charger["terminateCurrent"] = 4;
    charger["minBatteryVoltage"] = 270;
    charger["maxBatteryVoltage"] = 420;
    charger["minimumTemperature"] = 5;
    charger["maximumTemperature"] = 60;
    charger["maximumAmpereHours"] = 100;
    charger["maximumChargeTime"] = 600;
    charger["deratingRate"] = 10;
    charger["deratingReferenceTmp"] = 50;
    charger["hystereseStopTemp"] = 60;
    charger["hystereseResumeTemp"] = 40;

    JsonObject controls = root.createNestedObject("controls");
    controls["positionRegenMaximum"] = 10;
    controls["positionRegenMinimum"] = 20;
    controls["positionForwardStart"] = 30;
    controls["positionHalfPower"] = 80;
    controls["minimumRegen"] = 5;
    controls["maximumRegen"] = 80;
    controls["creepLevel"] = 10;
    controls["creepSpeed"] = 700;
    controls["brakeMinimumRegen"] = 20;
    controls["brakeMaximumRegen"] = 400;
    controls["slewRate"] = 100;
    controls["brakeHold"] = 50;

    JsonObject dashboard = root.createNestedObject("dashboard");
    dashboard["torqueRange"] = "-220,220";
    dashboard["rpmRange"] = "0,8000";
    dashboard["currentRange"] = "-200,200";
    dashboard["motorTempRange"] = "0,90,120";
    dashboard["controllerTempRange"] = "0,60,80";
    dashboard["batteryRangeLow"] = "297,357,368";
    dashboard["batteryRangeHigh"] = "387,405,418";
    dashboard["socRange"] = "0,20,100";
    dashboard["powerRange"] = "-150,150";
    dashboard["chargeInputRange"] = "0,56";
    dashboard["cruiseUseRpm"] = true;
    dashboard["cruiseSpeedStep"] = 100;
    dashboard["cruiseSpeedSet"] = 0;

    JsonObject dcdc = root.createNestedObject("dcdc");
    dcdc["dcDcMode"] = 0;
    dcdc["lowVoltageCommand"] = 13.5;
    dcdc["hvUndervoltageLimit"] = 270;
    dcdc["lvBuckCurrentLimit"] = 150;
    dcdc["hvBuckCurrentLimit"] = 20;
    dcdc["highVoltageCommand"] = 400;
    dcdc["lvUndervoltageLimit"] = 11.5;
    dcdc["lvBoostCurrentLimit"] = 150;
    dcdc["hvBoostCurrentLimit"] = 20;

    JsonObject devices = root.createNestedObject("devices");
    devices["x1000"] = 1;
    devices["x1001"] = 1;
    devices["x1002"] = 1;
    devices["x1010"] = 1;
    devices["x1020"] = 1;
    devices["x1022"] = 1;
    devices["x1031"] = 1;
    devices["x1032"] = 1;
    devices["x1033"] = 1;
    devices["x1034"] = 0;
    devices["x1040"] = 1;
    devices["x2000"] = 0;
    devices["x2001"] = 1;
    devices["x3000"] = 1;
    devices["x5001"] = 0;
    devices["x5003"] = 1;
    devices["x6000"] = 0;
    devices["x6500"] = 1;

    JsonObject inputs = root.createNestedObject("inputs");
    inputs["absInput"] = 3;
    inputs["enableInput"] = 0;
    inputs["reverseInput"] = 2;
    inputs["chargePwrAvailInput"] = 1;
    inputs["interlockInput"] = 255;

    JsonObject motor = root.createNestedObject("motor");
    motor["numberPotMeters"] = 2;
    motor["throttleSubType"] = 1;
    motor["minimumLevel"] = 111;
    motor["minimumLevel2"] = 222;
    motor["maximumLevel"] = 3333;
    motor["maximumLevel2"] = 4000;
    motor["brakeMinimumLevel"] = 11;
    motor["brakeMaximumLevel"] = 4001;

    motor["speedMax"] = 7000;
    motor["torqueMax"] = 220;
    motor["motorMode"] = 1;
    motor["nominalVolt"] = 399;
    motor["invertDirection"] = 1;

    motor["maxMechPowerMotor"] = 110;
    motor["maxMechPowerRegen"] = 109;
    motor["dcVoltLimitMotor"] = 300;
    motor["dcVoltLimitRegen"] = 410;
    motor["dcCurrentLimitMotor"] = 300;
    motor["dcCurrentLimitRegen"] = 200;
    motor["enableOscLimiter"] = 1;

    JsonObject outputs = root.createNestedObject("outputs");
    outputs["prechargeMillis"] = 1500;
    outputs["prechargeRelayOutput"] = 1;
    outputs["mainContactorOutput"] = 0;
    outputs["secondaryContOutput"] = 2;
    outputs["fastChargeContOutput"] = 3;
    outputs["enableMotorOutput"] = 4;
    outputs["enableChargerOutput"] = 5;
    outputs["enableDcDcOutput"] = 6;
    outputs["enableHeaterOutput"] = 7;
    outputs["heaterValveOutput"] = 255;
    outputs["heaterPumpOutput"] = 255;
    outputs["coolingPumpOutput"] = 255;
    outputs["coolingFanOutput"] = 255;
    outputs["coolingTempOn"] = 50;
    outputs["coolingTempOff"] = 40;
    outputs["brakeLightOutput"] = 255;
    outputs["reverseLightOutput"] = 255;
    outputs["warningOutput"] = 255;
    outputs["powerLimitOutput"] = 255;
}
