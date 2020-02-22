/*
 * WebSocket.h
 *
 *  Created on: 17 Feb 2020
 *      Author: michaeln
 */

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

#include <ESPAsyncWebServer.h>
#include "Configuration.h"
#include "Logger.h"

class WebSocket
{
public:
    WebSocket();
    virtual ~WebSocket();
    void start(Configuration *configuration, AsyncWebServer *server);
    void loop();
    void onWebsocketEvent(AsyncWebSocket *server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

private:
    void sendUpdate();

    Configuration *config;
    AsyncWebSocket *webSocketHandler;
    bool connected;
};

#endif /* WEBSOCKET_H_ */
