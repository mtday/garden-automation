
#include <Arduino.h>
#include <Ethernet.h>
#include <WiFi.h>
#include "net/Network.hpp"


static Network *network;

Network::Network() {
}

Network *Network::get() {
    if (!network) {
        network = new Network();
    }
    return network;
}

IPAddress Network::getIp() {
    return ip;
}

bool Network::setup() {
    return connect();
}

bool Network::loop() {
    if (!isConnected()) {
        return connect();
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

    Serial.printf("INFO:  Connecting to wifi network: %s\n", WIFI_SSID);
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
                Serial.printf("INFO:  Connected successfully, IP is: %s\n", ip.toString().c_str());
                return true;

            case WL_NO_SSID_AVAIL:
            case WL_CONNECT_FAILED:
            case WL_CONNECTION_LOST:
            case WL_DISCONNECTED:
                Serial.println("ERROR: Connection failed");
                return false;

            case WL_NO_SHIELD:
            case WL_SCAN_COMPLETED:
                // not expected in `WIFI_MODE_STA`
                Serial.println("ERROR: Connection failed, unexpected status");
                return false;

            case WL_IDLE_STATUS:
                const ulong now = millis();
                const ulong wait_time = now - start;
                if (wait_time > WIFI_CONNECT_TIMEOUT) {
                    // already waited too long
                    Serial.println("ERROR: Connection failed, timed out");
                    WiFi.disconnect();
                    return false;
                }
                // delay a little to wait for connection
                delay(WIFI_CONNECT_TIMEOUT - wait_time > 10 ? 10 : WIFI_CONNECT_TIMEOUT - wait_time);
        }
    } while (true);
}
