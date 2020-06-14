
#ifndef _GARDEN_AUTOMATION_NTPTIME_H
#define _GARDEN_AUTOMATION_NTPTIME_H


#include <time.h>


class NTPTime
{
public:
    NTPTime();

    void setup();
    time_t now();
};


#endif
