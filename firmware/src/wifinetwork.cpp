
#include <Arduino.h>
#include <Ethernet.h>
#include <WiFi.h>
#include "wifinetwork.h"


WiFiNetwork::WiFiNetwork(const char *ssid, const char *password, const ulong connectTimeout)
{
    WiFiNetwork::ssid = ssid;
    WiFiNetwork::password = password;
    WiFiNetwork::connectTimeout = connectTimeout;
}

uint8_t *WiFiNetwork::getMac()
{
    return mac;
}

IPAddress WiFiNetwork::getIp()
{
    return ip;
}

bool WiFiNetwork::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiNetwork::connect()
{
    if (isConnected()) {
        return true;
    }

    WiFi.mode(WIFI_MODE_STA);
    const ulong start = millis();
    do {
        const wl_status_t status = WiFi.begin(ssid, password);
        switch (status) {
            case WL_CONNECTED:
                ip = WiFi.localIP();
                WiFi.macAddress(mac);
                Ethernet.begin(mac, ip);
                return true;

            case WL_NO_SSID_AVAIL:
            case WL_CONNECT_FAILED:
            case WL_CONNECTION_LOST:
            case WL_DISCONNECTED:
                return false;

            case WL_NO_SHIELD:
            case WL_SCAN_COMPLETED:
                // not expected in `WIFI_MODE_STA`
                return false;

            case WL_IDLE_STATUS:
                const ulong now = millis();
                const ulong wait_time = now - start;
                if (wait_time > connectTimeout) {
                    // already waited too long
                    WiFi.disconnect();
                    return false;
                }
                // delay a little to wait for connection
                delay(connectTimeout - wait_time > 10 ? 10 : connectTimeout - wait_time);
        }
    } while (true);
}

bool WiFiNetwork::disconnect()
{
    if (isConnected()) {
        WiFi.disconnect();
        return true;
    }
    return false;
}
