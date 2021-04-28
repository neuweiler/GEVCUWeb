/*
 * FSHandler.h
 *
 *  Created on: 28 Apr 2021
 *      Author: michael
 */

#ifndef FSHANDLER_H_
#define FSHANDLER_H_

#include <SD_MMC.h>
#include <SPIFFS.h>

#include "Logger.h"

class FSHandler {
public:
	FSHandler();
	virtual ~FSHandler();
	FS *getFS();
private:
	void init();

	fs::FS *fileSystem;
};

extern FSHandler fsHandler;

#endif /* FSHANDLER_H_ */
