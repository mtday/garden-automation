
#ifndef _NETWORK_TIME_HPP
#define _NETWORK_TIME_HPP


#include <time.h>
#include "Device.hpp"


class NetworkTime
{
protected:
    bool setup();

public:
    NetworkTime();
    static bool get(NetworkTime **ref);

    String isotime();
};


#endif
