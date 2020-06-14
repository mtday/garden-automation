
#include <Arduino.h>
#include "device.h"


static Device *device;

static void messageHandler(char *topic, uint8_t *payload, uint length) {
    if (device) {
        Serial.print("Message: ");
        Serial.print(topic);
        Serial.print(" => ");
        for (int i = 0; i < length; i++) {
            Serial.print((char) payload[i]);
        }
        Serial.println();

        StaticJsonDocument<1024> message;
        deserializeJson(message, payload, length);

        if (!device->handleMessage(topic, message)) {
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