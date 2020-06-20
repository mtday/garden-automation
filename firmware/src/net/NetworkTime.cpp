
#include <Arduino.h>
#include <Time.h>
#include "net/NetworkTime.hpp"


static NetworkTime *networkTime;

NetworkTime::NetworkTime() {
}

bool NetworkTime::get(NetworkTime **ref, DeviceType deviceType) {
    if (networkTime) {
        *ref = networkTime;
        return true;
    }
    if (deviceType != DeviceTypeController) {
        *ref = NULL;
        return true;
    }
    networkTime = new NetworkTime();
    if (!networkTime->setup()) {
        networkTime = *ref = NULL;
        return false;
    }
    *ref = networkTime;
    return true;
}

bool NetworkTime::setup() {
    Serial.println("INFO:  Configuring network time");
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    return true;
}

time_t NetworkTime::now() {
    return ::now();
}
