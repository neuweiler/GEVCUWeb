GEVCU - ESP32 Web
=================
by Michael Neuweiler

A replacement for the GEVCU iChip. Use an ESP32 to connect via the existing serial interface
to the GEVCU. It provide the GEVCU website and data via websocket to a web browser.

Requires the following libraries:
* ArduinoJson
* https://github.com/me-no-dev/AsyncTCP
* https://github.com/me-no-dev/ESPAsyncWebServer
* ESPmDNS
* FS
* SD_MMC
* SPIFFS
* WiFi

Requires the following hardware:
* ESP32 - if possible with SD card reader integrated
* SD card with FAT filesystem to store the web site data on (you may also use 2MB SPIFFS instead).
Upload the contents of the directory "data" to the root of the file system.
* GEVCU (General Electric Vehicle Control Unit) to connect to via serial interface.

Note: To ensure flawless operation, increase the "max_files" config in SD_MMC.cpp from 5 to 20 !
      (in SDMMCFS::begin() )
