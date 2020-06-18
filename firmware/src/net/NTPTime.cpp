
#include <Arduino.h>
#include <Time.h>
#include "net/NtpTime.hpp"


static NtpTime *ntpTime;

NtpTime::NtpTime() {
}

NtpTime *NtpTime::get() {
    if (!ntpTime) {
        ntpTime = new NtpTime();
    }
    return ntpTime;
}

bool NtpTime::setup() {
    Serial.println("INFO:  Configuring NTP time");
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    return true;
}

time_t NtpTime::now() {
    return ::now();
}
