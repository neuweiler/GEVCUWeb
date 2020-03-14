/*
 * GevcuParams.cpp
 *
 *  Created on: 13 Mar 2020
 *      Author: michaeln
 */

#include "GevcuParams.h"

GevcuParams::GevcuParams()
{
    // TODO Auto-generated constructor stub

}

GevcuParams::~GevcuParams()
{
    // TODO Auto-generated destructor stub
}

/**
 * Return the instance of the singleton
 */
GevcuParams *GevcuParams::getInstance()
{
    static GevcuParams instance;
    return &instance;
}

void GevcuParams::toJson(JsonObject &root) {
}
