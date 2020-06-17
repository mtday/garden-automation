
#include <Arduino.h>
#include <Time.h>
#include "net/NTPTime.hpp"


static NTPTime *ntpTime;

NTPTime::NTPTime() {
}

NTPTime *NTPTime::get() {
    if (!ntpTime) {
        ntpTime = new NTPTime();
    }
    return ntpTime;
}

bool NTPTime::setup() {
    Serial.println("INFO:  Configuring NTP time");
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    return true;
}

time_t NTPTime::now() {
    return ::now();
}
