/*
 * HeartBeat.h
 *
 *  Created on: 18 Feb 2020
 *      Author: michaeln
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include <Arduino.h>
#include "Configuration.h"

class HeartBeat
{
public:
    HeartBeat();
    virtual ~HeartBeat();
    void start(Configuration *config);
    void loop();

private:
    Configuration *config;
    uint32_t timestamp;
    bool led;
    bool connected;
};

#endif /* HEARTBEAT_H_ */
