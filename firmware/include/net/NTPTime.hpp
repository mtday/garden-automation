
#ifndef _NTP_TIME_HPP
#define _NTP_TIME_HPP


#include <time.h>


class NtpTime {
public:
    NtpTime();
    static NtpTime *get();

    bool setup();
    time_t now();
};


#endif
