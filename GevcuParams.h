/*
 * GevcuParams.h
 *
 *  Created on: 16 Feb 2020
 *      Author: michaeln
 */

#ifndef GEVCUPARAMS_H_
#define GEVCUPARAMS_H_

#include <Arduino.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "Logger.h"

class ConfigType
{
public:
    String key;
    String value;
    ConfigType()
    {
    }
    ConfigType(String key, String value)
    {
        this->key = key;
        this->value = value;
    }
};

class GevcuParams
{
public:
    enum Params
    {
        torqueActual, // int16_t
        throttle, // int16_t
        speedActual, // int16_t
        dcVoltage, // uint16_t
        dcCurrent, // int16_t
        acCurrent, // int16_t
        bitfieldMotor, // uint32_t
        bitfieldBms, // uint32_t
        bitfieldIO, // uint32_t
        systemState, // uint16_t
        temperatureMotor, // int16_t
        temperatureController, // int16_t
        mechanicalPower, // int32_t
        dcDcHvVoltage, // uint16_t
        dcDcLvVoltage, // uint16_t
        dcDcHvCurrent, // int16_t
        dcDcLvCurrent, // int16_t
        dcDcTemperature, // int16_t
        chargerInputVoltage, // uint16_t
        chargerInputCurrent, // uint16_t
        chargerBatteryVoltage, // uint16_t
        chargerBatteryCurrent, // uint16_t
        chargerTemperature, // int16_t
        maximumSolarCurrent, // int16_t
        chargeHoursRemain, // uint16_t
        chargeMinsRemain, // uint16_t
        chargeLevel, // uint16_t
        flowCoolant, // uint32_t
        flowHeater, // uint32_t
        heaterPower, // uint16_t
        temperatureBattery, // int16_t [CFG_NUMBER_BATTERY_TEMPERATURE_SENSORS];
        temperatureCoolant, // int16_t
        temperatureHeater, // int16_t
        temperatureExterior, // int16_t
        powerSteering, // bool
        enableRegen, // bool
        enableHeater, // bool
        enableCreep, // bool
        cruiseControlSpeed, // int16_t
        cruiseControlEnable, // bool

        packResistance, // uint16_t
        packHealth, // uint8_t
        packCycles, // uint16_t
        soc, // uint16_t
        dischargeLimit, // uint16_t
        chargeLimit, // uint16_t
        chargeAllowed, // bool
        dischargeAllowed, // bool
        lowestCellTemp, // int16_t
        highestCellTemp, // int16_t
        lowestCellVolts, // uint16_t
        highestCellVolts, // uint16_t
        averageCellVolts, // uint16_t
        deltaCellVolts, // uint16_t
        lowestCellResistance, // uint16_t
        highestCellResistance, // uint16_t
        averageCellResistance, // uint16_t
        deltaCellResistance, // uint16_t
        lowestCellTempId, // uint8_t
        highestCellTempId, // uint8_t
        lowestCellVoltsId, // uint8_t
        highestCellVoltsId, // uint8_t
        lowestCellResistanceId, // uint8_t
        highestCellResistanceId, // uint8_t
        bmsTemperature // uint8_t
    };

    virtual ~GevcuParams();
    static GevcuParams *getInstance();
    void getConfig(JsonObject &root);
private:
    GevcuParams();
    GevcuParams(GevcuParams const&); // copy disabled
    void operator=(GevcuParams const&); // assigment disabled

//    const byte HASH_SIZE = 120;
//    ConfigType config[HASH_SIZE];
};

#endif /* GEVCUPARAMS_H_ */
