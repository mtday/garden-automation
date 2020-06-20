
#include <Arduino.h>
#include <Time.h>
#include "net/NetworkTime.hpp"


static NetworkTime *networkTime;

NetworkTime::NetworkTime() {
}

bool NetworkTime::get(NetworkTime **ref) {
    if (networkTime) {
        *ref = networkTime;
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
    Serial.printf("INFO:  Current time is: %s\n", isotime().c_str());
    return true;
}

time_t NetworkTime::now() {
    return ::now();
}

String NetworkTime::isotime() {
    const time_t time = now();
    char timestr[20];
    snprintf(timestr, 20, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            year(time), month(time), day(time), hour(time), minute(time), second(time));
    return String(timestr);
}
