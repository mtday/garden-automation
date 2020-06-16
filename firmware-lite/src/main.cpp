
#include <Arduino.h>
#include <WiFi.h>

#include "device/Device.hpp"


static Device *device;

void setup() {
    Serial.begin(SERIAL_BAUD);

    device = Device::createDevice();
    device->setup();
}

void loop() {
    device->loop();
}
