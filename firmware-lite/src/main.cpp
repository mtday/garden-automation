
#include <Arduino.h>
#include <WiFi.h>

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "TankVolumeDevice.hpp"


static Device *device;

void setup() {
    Serial.begin(SERIAL_BAUD);

    device = DeviceManager().getDevice();
    device->setup();
}

void loop() {
    device->loop();
}
