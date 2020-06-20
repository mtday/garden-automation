
#ifndef _NETWORK_HPP
#define _NETWORK_HPP


#include <stdint.h>
#include <IPAddress.h>
#include "Device.hpp"


class Network {
private:
    IPAddress ip;

protected:
    bool setup();

    bool isConnected();
    bool connect();

public:
    Network();
    static bool get(Network **ref);

    IPAddress getIp();

    bool loop();
};


#endif
