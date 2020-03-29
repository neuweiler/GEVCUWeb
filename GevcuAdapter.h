/*
 * GevcuAdapter.h
 *
 *  Created on: 17 Feb 2020
 *      Author: michaeln
 */

#ifndef GEVCUADAPTER_H_
#define GEVCUADAPTER_H_

#include "Configuration.h"
#include "ArrayMap.h"
#include "Logger.h"

class GevcuAdapter
{
public:
    GevcuAdapter();
    virtual ~GevcuAdapter();
    void start(Configuration *config);
    void loop();
    String getConfigParameter(String key);
private:
    void processInput(char *input);
    void loadDummyData();
    Configuration *config;
    ArrayMap<String, String> gevcuConfig;
};

extern GevcuAdapter gevcuAdapter;

#endif /* GEVCUADAPTER_H_ */
