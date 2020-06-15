
#include <Arduino.h>
#include "device.h"


static Device *device;

static void messageHandler(char *topic, uint8_t *payload, uint length) {
    if (device) {
        char m[length + 1];
        snprintf(m, length, "%s", payload);
        Serial.printf("Received: %s => %s\n", topic, m);

        StaticJsonDocument<1024> message;
        deserializeJson(message, payload, length);

        if (!device->handleMessage(topic, message)) {
            Serial.println("Failed to handle message");
        }
    }
}

void setup() {
    device = new Device(
        WIFI_SSID,
        WIFI_PASSWORD,
        WIFI_CONNECT_TIMEOUT,
        MQTT_BROKER_IP,
        MQTT_BROKER_PORT,
        messageHandler
    );

    device->setup();
}

void loop() {
    device->loop();
}