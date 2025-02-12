#include <WiFi.h>
#include "main.h"
#include "network.h"
#include "display.h"
#include "logging.h"
#include "constants.h"

WebServer server(80);

void connectWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    log_state = 1;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    log_state = 2;
}

void setupServer() {
    server.on("/download", handleDownload);
    server.begin();
    }

void handleDownload() {
    if (SD.exists(LOG_FILENAME)) {
        File file = SD.open(LOG_FILENAME);
        server.streamFile(file, "application/octet-stream");
        file.close();
    } else {
        server.send(404, "text/plain", "File not found");
    }
}
