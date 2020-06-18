
#include <Arduino.h>
#include <Time.h>
#include "net/NetworkTime.hpp"


static NetworkTime *networkTime;

NetworkTime::NetworkTime() {
}

NetworkTime *NetworkTime::get() {
    if (!networkTime) {
        networkTime = new NetworkTime();
    }
    return networkTime;
}

bool NetworkTime::setup() {
    Serial.println("INFO:  Configuring network time");
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    return true;
}

time_t NetworkTime::now() {
    return ::now();
}
