/*
 * WebSocket.cpp
 *
 Copyright (c) 2020 Michael Neuweiler

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */


#include "WebSocket.h"

WebSocket webSocket;

WebSocket::WebSocket()
{
    config = NULL;
    connected = false;
    observer = NULL;
    webSocketHandler = new AsyncWebSocket("/ws");
}

WebSocket::~WebSocket()
{
    if (webSocketHandler) {
        free(webSocketHandler);
        webSocketHandler = NULL;
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    webSocket.onWebsocketEvent(server, client, type, arg, data, len);
}

void WebSocket::start(Configuration *configuration, AsyncWebServer *server)
{
    config = configuration;
    connected = false;
    webSocketHandler->onEvent(onEvent);
    server->addHandler(webSocketHandler);
    logger.info("WebSocket configured");
}

void WebSocket::onWebsocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT) {
        connected = true;
        logger.info("ws[%s][%u] connect", server->url(), client->id());
        observer->event("connected");
    } else if (type == WS_EVT_DISCONNECT) {
        connected = false;
        logger.info("ws[%s][%u] disconnect", server->url(), client->id());
        observer->event("disconnected");
    } else if (type == WS_EVT_ERROR) {
        logger.warn("ws[%s][%u] error(%u): %s", server->url(), client->id(), *((uint16_t*) arg), (char*) data);
    } else if (type == WS_EVT_PONG) {
        logger.debug("ws[%s][%u] pong[%u]: %s", server->url(), client->id(), len, (len) ? (char*) data : "");
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*) arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len) {
            //the whole message is in a single frame and we got all of it's data
            if (logger.isDebug()) {
                logger.debug("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
            }

            if (info->opcode == WS_TEXT) {
                for (size_t i = 0; i < info->len; i++) {
                    msg += (char) data[i];
                }
            } else {
                char buff[3];
                for (size_t i = 0; i < info->len; i++) {
                    sprintf(buff, "%02x ", (uint8_t) data[i]);
                    msg += buff;
                }
            }

            if (msg.equals("ping")) {
                webSocketHandler->textAll("pong");
            } else if (observer) {
                observer->event(msg);
            }
        }
    }
}

bool WebSocket::isConnected() {
    return connected;
}

void WebSocket::subscribe(WebSocketObserver *observer)
{
    this->observer = observer;
}

void WebSocket::send(String data)
{
    webSocketHandler->textAll(data);
}

void WebSocket::loop()
{
    webSocketHandler->cleanupClients();
}
