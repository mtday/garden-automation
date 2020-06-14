
#include <Arduino.h>
#include "device.h"


static Device *device;

static void messageHandler(char* topic, uint8_t* payload, uint length) {
    if (device) {
        device->handleMessage(topic, payload, length);
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