/*
 * WebServer.h
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

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <SD_MMC.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Logger.h"
#include "GevcuAdapter.h"

class WebServer {
public:
	WebServer();
	virtual ~WebServer();
	void init();
	AsyncWebServer* getWebServer();
	void deleteFile(String file);
	void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len,
			bool final);
	String getUploadPath();

private:
	void setupFilesystem();
	void setupWebserver();
	String fileList(String path);

	AsyncWebServer *server;
	static const char *MIME_TYPE_JSON;
	static fs::FS *fileSystem;
	File uploadFile;
	String uploadPath;
};

extern WebServer webServer;

#endif /* WEBSERVER_H_ */
