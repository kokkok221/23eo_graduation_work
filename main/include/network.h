#ifndef NETWORK_H
#define NETWORK_H

#include <WebServer.h>

extern WebServer server;

void connectWiFi(const char* ssid, const char* password);
void setupServer();
void handleDownload();

#endif // NETWORK_H
