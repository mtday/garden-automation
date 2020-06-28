
#include <Arduino.h>
#include <time.h>
#include "net/NetworkTime.hpp"


static NetworkTime *networkTime;

NetworkTime::NetworkTime()
{
}

bool NetworkTime::get(NetworkTime **ref)
{
    if (networkTime)
    {
        *ref = networkTime;
        return true;
    }
    networkTime = new NetworkTime();
    if (!networkTime->setup())
    {
        networkTime = *ref = NULL;
        return false;
    }
    *ref = networkTime;
    return true;
}

bool NetworkTime::setup()
{
    Serial.println("INFO:  Configuring network time");
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    Serial.printf("INFO:  Current time is: %s\n", isotime().c_str());
    return true;
}

String NetworkTime::isotime()
{
    struct tm tm;
    getLocalTime(&tm, 5000);

    char timestr[22];
    snprintf(timestr, 22, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    return String(timestr);
}
