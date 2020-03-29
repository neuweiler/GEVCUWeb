GEVCU - ESP Web
===============

A replacement for the GEVCU iChip. Use a ESP32 or ESP8266 to connect via the existing serial interface
to the GEVCU and provide the GEVCU website and data to a web browser.

Requires the following libraries:
* https://github.com/me-no-dev/AsyncTCP (for ESP32)
* https://github.com/me-no-dev/ESPAsyncTCP (for ESP8266)
* https://github.com/me-no-dev/ESPAsyncWebServer

Note: To ensure flawless operation, increase the max_files config in SD_MMC.cpp from 5 to 20 !
      (in SDMMCFS::begin() )
