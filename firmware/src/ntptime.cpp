
#include <Arduino.h>
#include <Time.h>
#include "ntptime.h"


NTPTime::NTPTime()
{
}

void NTPTime::setup()
{
    configTime(0, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
}

time_t NTPTime::now()
{
    return ::now();
}
