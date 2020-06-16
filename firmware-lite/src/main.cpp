
#include <Arduino.h>
#include <WiFi.h>

#include "Device.hpp"


static Device *device;

void setup() {
    Serial.begin(SERIAL_BAUD);

    device = Device::getDevice();
    device->setup();
}

void loop() {
    device->loop();
}
