
#include <Arduino.h>
#include <Ethernet.h>
#include <WiFi.h>
#include "net/Network.hpp"


static Network *network;

Network::Network() {
}

bool Network::get(Network **ref) {
    if (network) {
        *ref = network;
        return true;
    }
    network = new Network();
    if (!network->setup()) {
        network = *ref = NULL;
        return false;
    }
    *ref = network;
    return true;
}

IPAddress Network::getIp() {
    return ip;
}

bool Network::setup() {
    return connect();
}

bool Network::loop() {
    if (!isConnected() && !connect()) {
        Serial.println("ERROR: Failed to reconnect to WiFi network");
        return false;
    }
    return true;
}

bool Network::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool Network::connect() {
    if (isConnected()) {
        return true;
    }

    Serial.printf("INFO:  Connecting to WiFi network: %s\n", WIFI_SSID);
    WiFi.mode(WIFI_MODE_STA);
    const ulong start = millis();
    do {
        const wl_status_t status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        switch (status) {
            case WL_CONNECTED:
                ip = WiFi.localIP();
                uint8_t mac[6];
                WiFi.macAddress(mac);
                Ethernet.begin(mac, ip);
                Serial.printf("INFO:  WiFi connected successfully, IP is: %s\n", ip.toString().c_str());
                return true;

            case WL_NO_SSID_AVAIL:
            case WL_CONNECT_FAILED:
            case WL_CONNECTION_LOST:
            case WL_DISCONNECTED:
                Serial.printf("ERROR: WiFi connection failed with status %d\n", status);
                return false;

            case WL_NO_SHIELD:
            case WL_SCAN_COMPLETED:
                // not expected in `WIFI_MODE_STA`
                Serial.printf("ERROR: WiFi connection failed, unexpected status %d\n", status);
                return false;

            case WL_IDLE_STATUS:
                const ulong now = millis();
                const ulong waitTime = now - start;
                if (waitTime > WIFI_CONNECT_TIMEOUT) {
                    // already waited too long
                    Serial.println("ERROR: WiFi connection failed, timed out");
                    WiFi.disconnect();
                    return false;
                }
                // delay a little to wait for connection
                delay(WIFI_CONNECT_TIMEOUT - waitTime > 10 ? 10 : WIFI_CONNECT_TIMEOUT - waitTime);
        }
    } while (true);
}
