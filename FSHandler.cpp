/*
 * FSHandler.cpp
 *
 *  Created on: 28 Apr 2021
 *      Author: michael
 */

#include "FSHandler.h"

FSHandler fsHandler;

FSHandler::FSHandler() {
	fileSystem = NULL;
}

FSHandler::~FSHandler() {
}

void FSHandler::init() {
	if (SD_MMC.begin()) {
		uint8_t cardType = SD_MMC.cardType();
		if (cardType != CARD_NONE) {
			logger.info("SD card mount successful");
			logger.debug("SD_MMC Card Type: %s",
					(cardType == CARD_MMC ?
							"MMC" : (cardType == CARD_SD ? "SDSC" : (cardType == CARD_SDHC ? "SDHC" : "unknown"))));
			fileSystem = &SD_MMC;
			return;
		}
		logger.error("No SD_MMC card attached");
	}
	// try SPIFFS as fall-back
	if (!SPIFFS.begin()) {
		logger.error("SPIFFS Mount failed");
	} else {
		logger.info("SPIFFS Mount successful");
		fileSystem = &SPIFFS;
	}
}

FS* FSHandler::getFS() {
	if (!fileSystem) {
		init();
	}
	return fileSystem;
}
