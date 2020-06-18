
#ifndef _NETWORK_TIME_HPP
#define _NETWORK_TIME_HPP


#include <time.h>


class NetworkTime {
public:
    NetworkTime();
    static NetworkTime *get();

    bool setup();
    time_t now();
};


#endif
