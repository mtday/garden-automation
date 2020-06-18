
#ifndef _NETWORK_HPP
#define _NETWORK_HPP


#include <stdint.h>
#include <IPAddress.h>


class Network {
private:
    IPAddress ip;

protected:
    bool isConnected();
    bool connect();

public:
    Network();
    static Network *get();

    IPAddress getIp();

    bool setup();
    bool loop();
};


#endif
