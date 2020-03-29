#include "GEVCU-ESP32Web.h"

Configuration config;
WebSocket webSocket;
HeartBeat heartBeat;

void setup()
{
    logger.info("setup");

    config.load();

    webServer.start(&config);
    webSocket.start(&config, webServer.getWebServer());

    gevcuAdapter.start(&config);

    heartBeat.start(&config);
}

void loop()
{
    delay(1000);
    gevcuAdapter.loop();
    webSocket.loop();
    heartBeat.loop();
}
