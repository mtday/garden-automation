
#ifndef _GARDEN_AUTOMATION_NET_NTPTIME_HPP
#define _GARDEN_AUTOMATION_NET_NTPTIME_HPP


#include <time.h>


class NTPTime
{
public:
    NTPTime();
    static NTPTime *get();

    bool setup();
    time_t now();
};


#endif
