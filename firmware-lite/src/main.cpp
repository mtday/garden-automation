
#include <Arduino.h>
#include <WiFi.h>

#include "Runner.hpp"


void setup() {
    Serial.begin(SERIAL_BAUD);

    if (!Runner::get()->setup()) {
        Serial.println("ERROR: Restarting due to setup failure");
        delay(3000);
        Runner::get()->restart();
    }
}

void loop() {
    if (!Runner::get()->loop()) {
        Serial.println("ERROR: Restarting due to loop failure");
        delay(3000);
        Runner::get()->restart();
    }
}
