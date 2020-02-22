/*
 * GevcuAdapter.h
 *
 *  Created on: 17 Feb 2020
 *      Author: michaeln
 */

#ifndef GEVCUADAPTER_H_
#define GEVCUADAPTER_H_

#include "Configuration.h"

class GevcuAdapter
{
public:
    GevcuAdapter();
    virtual ~GevcuAdapter();
    void start(Configuration *config);
    void loop();
private:
    void processInput(char *input);
    Configuration *config;
};

#endif /* GEVCUADAPTER_H_ */
