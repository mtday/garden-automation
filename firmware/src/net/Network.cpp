
#include <Arduino.h>
#include <Ethernet.h>
#include <WiFi.h>
#include "net/Network.hpp"


static Network *network;

Network::Network()
{
}

bool Network::get(Network **ref)
{
    if (network)
    {
        *ref = network;
        return true;
    }
    network = new Network();
    if (!network->setup())
    {
        network = *ref = NULL;
        return false;
    }
    *ref = network;
    return true;
}

IPAddress Network::getIp()
{
    return ip;
}

bool Network::setup()
{
    return connect();
}

bool Network::loop()
{
    if (!isConnected() && !connect())
    {
        Serial.println("ERROR: Failed to reconnect to WiFi network");
        return false;
    }
    return true;
}

bool Network::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool Network::connect()
{
    if (isConnected())
    {
        return true;
    }

    Serial.printf("INFO:  Connecting to WiFi network: %s\n", WIFI_SSID);
    WiFi.mode(WIFI_MODE_STA);
    const ulong start = millis();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    do
    {
        const wl_status_t status = WiFi.status();
        switch (status)
        {
            case WL_CONNECTED:
                ip = WiFi.localIP();
                uint8_t mac[6];
                WiFi.macAddress(mac);
                Ethernet.begin(mac, ip);
                Serial.printf("INFO:  WiFi connected successfully in %ld millis, IP is: %s\n",
                        millis() - start, ip.toString().c_str());
                return true;

            case WL_NO_SSID_AVAIL:
                Serial.printf("ERROR: WiFi connection failed: NO SSID AVAILABLE\n");
                return false;

            case WL_CONNECT_FAILED:
                Serial.printf("ERROR: WiFi connection failed: CONNECT FAILED\n");
                return false;

            case WL_CONNECTION_LOST:
                Serial.printf("ERROR: WiFi connection failed: CONNECTION LOST\n");
                return false;

            case WL_NO_SHIELD:
                Serial.printf("ERROR: WiFi connection failed, unexpected status NO SHIELD\n");
                return false;

            case WL_SCAN_COMPLETED:
                Serial.printf("ERROR: WiFi connection failed, unexpected status SCAN COMPLETED\n");
                return false;

            case WL_DISCONNECTED: // usually the first status returned
            case WL_IDLE_STATUS:
                const ulong waitTime = millis() - start;
                if (waitTime > WIFI_CONNECT_TIMEOUT)
                {
                    // already waited too long
                    Serial.println("ERROR: WiFi connection failed, timed out");
                    WiFi.disconnect();
                    return false;
                }
                // delay a little to wait for connection
                delay(WIFI_CONNECT_TIMEOUT - waitTime > 100 ? 100 : WIFI_CONNECT_TIMEOUT - waitTime);
        }
    } while (true);
}
