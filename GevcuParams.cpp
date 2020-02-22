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

String GevcuParams::findConfig(String key)
{
    /*    for (int i = 0; i < HASH_SIZE; i++) {
     if (key.equals(config[i].key)) {
     return config[i].value;
     }
     }*/

    //TODO replace hard-coded values with values provided by GEVCU

     // charger.js
     if (key == "%maximumInputCurrent%") {
     return "32";
     } else if (key == "constantCurrent") {
     return "10";
     } else if (key == "constantVoltage") {
     return "416.3";
     } else if (key == "terminateCurrent") {
     return "4";
     } else if (key == "minBatteryVoltage") {
     return "270";
     } else if (key == "maxBatteryVoltage") {
     return "420";
     } else if (key == "minimumTemperature") {
     return "5";
     } else if (key == "maximumTemperature") {
     return "60";
     } else if (key == "maximumAmpereHours") {
     return "100";
     } else if (key == "maximumChargeTime") {
     return "600";
     } else if (key == "deratingRate") {
     return "10";
     } else if (key == "deratingReferenceTmp") {
     return "50";
     } else if (key == "hystereseStopTemp") {
     return "60";
     } else if (key == "hystereseResumeTemp") {
     return "40";

     // controls.js
     } else if (key == "positionRegenMaximum") {
     return "10";
     } else if (key == "positionRegenMinimum") {
     return "20";
     } else if (key == "positionForwardStart") {
     return "30";
     } else if (key == "positionHalfPower") {
     return "80";
     } else if (key == "minimumRegen") {
     return "5";
     } else if (key == "maximumRegen") {
     return "80";
     } else if (key == "creepLevel") {
     return "10";
     } else if (key == "creepSpeed") {
     return "700";
     } else if (key == "brakeMinimumRegen") {
     return "20";
     } else if (key == "brakeMaximumRegen") {
     return "4001";
     } else if (key == "slewRate") {
     return "100";
     } else if (key == "brakeHold") {
     return "50";

     // dashboard.js
     } else if (key == "torqueRange") {
     return "-220,220";
     } else if (key == "rpmRange") {
     return "0,8000";
     } else if (key == "currentRange") {
     return "-200,200";
     } else if (key == "motorTempRange") {
     return "0,90,120";
     } else if (key == "controllerTempRange") {
     return "0,60,80";
     } else if (key == "batteryRangeLow") {
     return "297,357,368";
     } else if (key == "batteryRangeHigh") {
     return "387,405,418";
     } else if (key == "socRange") {
     return "0,20,100";
     } else if (key == "powerRange") {
     return "-150,150";
     } else if (key == "chargeInputLevels") {
     return "5,10,13,16";

     // dcdc.js
     } else if (key == "dcDcMode") {
     return "0";
     } else if (key == "lowVoltageCommand") {
     return "13.5";
     } else if (key == "hvUndervoltageLimit") {
     return "270";
     } else if (key == "lvBuckCurrentLimit") {
     return "150";
     } else if (key == "hvBuckCurrentLimit") {
     return "20";
     } else if (key == "highVoltageCommand") {
     return "400";
     } else if (key == "lvUndervoltageLimit") {
     return "11.5";
     } else if (key == "lvBoostCurrentLimit") {
     return "150";
     } else if (key == "hvBoostCurrentLimit") {
     return "20";

     // devices.js
     } else if (key == "x1000") {
     return "1";
     } else if (key == "x1001") {
     return "1";
     } else if (key == "x1002") {
     return "1";
     } else if (key == "x1010") {
     return "1";
     } else if (key == "x1020") {
     return "1";
     } else if (key == "x1022") {
     return "1";
     } else if (key == "x1031") {
     return "1";
     } else if (key == "x1032") {
     return "1";
     } else if (key == "x1033") {
     return "1";
     } else if (key == "x1034") {
     return "0";
     } else if (key == "x1040") {
     return "1";
     } else if (key == "x2000") {
     return "0";
     } else if (key == "x2001") {
     return "1";
     } else if (key == "x3000") {
     return "1";
     } else if (key == "x5001") {
     return "0";
     } else if (key == "x5003") {
     return "1";
     } else if (key == "x6000") {
     return "0";
     } else if (key == "x6500") {
     return "1";
     } else if (key == "logLevel") {
     return "2";

     // inputs.js
     } else if (key == "absInput") {
     return "3";
     } else if (key == "enableInput") {
     return "0";
     } else if (key == "reverseInput") {
     return "2";
     } else if (key == "chargePwrAvailInput") {
     return "1";
     } else if (key == "interlockInput") {
     return "255";

     // motor.js
     } else if (key == "numberPotMeters") {
     return "2";
     } else if (key == "throttleSubType") {
     return "1";
     } else if (key == "minimumLevel") {
     return "111";
     } else if (key == "minimumLevel2") {
     return "222";
     } else if (key == "maximumLevel") {
     return "3333";
     } else if (key == "maximumLevel2") {
     return "4000";
     } else if (key == "brakeMinimumLevel") {
     return "11";
     } else if (key == "brakeMaximumLevel") {
     return "4001";
     } else if (key == "speedMax") {
     return "7000";
     } else if (key == "torqueMax") {
     return "220";
     } else if (key == "motorMode") {
     return "1";
     } else if (key == "nominalVolt") {
     return "399";
     } else if (key == "invertDirection") {
     return "1";
     } else if (key == "maxMechPowerMotor") {
     return "110";
     } else if (key == "maxMechPowerRegen") {
     return "109";
     } else if (key == "dcVoltLimitMotor") {
     return "300";
     } else if (key == "dcVoltLimitRegen") {
     return "410";
     } else if (key == "dcCurrentLimitMotor") {
     return "300";
     } else if (key == "dcCurrentLimitRegen") {
     return "200";
     } else if (key == "enableOscLimiter") {
     return "1";

     // outputs.js
     } else if (key == "prechargeMillis") {
     return "1500";
     } else if (key == "prechargeRelayOutput") {
     return "1";
     } else if (key == "mainContactorOutput") {
     return "0";
     } else if (key == "secondaryContOutput") {
     return "2";
     } else if (key == "fastChargeContOutput") {
     return "3";
     } else if (key == "enableMotorOutput") {
     return "4";
     } else if (key == "enableChargerOutput") {
     return "5";
     } else if (key == "enableDcDcOutput") {
     return "6";
     } else if (key == "enableHeaterOutput") {
     return "7";
     } else if (key == "heaterValveOutput") {
     return "255";
     } else if (key == "heaterPumpOutput") {
     return "255";
     } else if (key == "coolingPumpOutput") {
     return "255";
     } else if (key == "coolingFanOutput") {
     return "255";
     } else if (key == "coolingTempOn") {
     return "50";
     } else if (key == "coolingTempOff") {
     return "40";
     } else if (key == "brakeLightOutput") {
     return "255";
     } else if (key == "reverseLightOutput") {
     return "255";
     } else if (key == "warningOutput") {
     return "255";
     } else if (key == "powerLimitOutput") {
     return "255";
     }

     return String("0");
}

