
#include <Arduino.h>
#include <WiFi.h>

#include "Device.hpp"


void setup() {
    Serial.begin(SERIAL_BAUD);

    if (!Device::get()->setup()) {
        Serial.println("ERROR: Restarting due to setup failure");
        delay(3000);
        Device::get()->restart();
    }
}

void loop() {
    if (!Device::get()->loop()) {
        Serial.println("ERROR: Restarting due to loop failure");
        delay(3000);
        Device::get()->restart();
    }
}
