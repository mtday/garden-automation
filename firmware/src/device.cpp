
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "device.h"


Device::Device(
    const char *wifissid,
    const char *wifiPassword,
    const ulong wifiConnectTimeout,
    const char *mqttBrokerIP,
    const ushort mqttBrokerPort,
    std::function<void(char *, uint8_t *, uint)> messageHandler
) {
    uint8_t macAddress[6];
    WiFi.macAddress(macAddress);
    char deviceId[5];
    sprintf(deviceId, "%02x%02x", macAddress[4], macAddress[5]);

    id = deviceId;
    type = DEVICE_TYPE_UNKNOWN;

    wifiNetwork = new WiFiNetwork(wifissid, wifiPassword, wifiConnectTimeout);
    messenger = new Messenger(id, wifiNetwork, mqttBrokerIP, mqttBrokerPort, messageHandler);
    ntpTime = new NTPTime();
}

void Device::setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("BEGIN");

    if (!wifiNetwork->connect()) {
        Serial.println("Failed to connect to network, restarting in 5 seconds");
        delay(5000);
        esp_restart();
    }
    if (!messenger->connect()) {
        Serial.println("Failed to connect to MQTT broker, restarting in 5 seconds");
        delay(5000);
        esp_restart();
    }

    ntpTime->setup();
    Serial.printf("Current time: %ld\n", ntpTime->now());
}

void Device::loop()
{
    Serial.println("LOOP");
    messenger->loop();
    delay(1000);
}

void Device::handleMessage(char *topic, uint8_t *payload, uint length)
{
    Serial.print("MQTT: ");
    Serial.print(topic);
    Serial.print(" => ");

    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();

    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload, length);
    // TODO
}
