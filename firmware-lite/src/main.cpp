
#include <Arduino.h>
#include <WiFi.h>

#include "device/Device.hpp"
#include "device/DeviceManager.hpp"


static Device *device;

void setup() {
    Serial.begin(SERIAL_BAUD);

    device = DeviceManager().getDevice();
    device->setup();
}

void loop() {
    device->loop();
}
