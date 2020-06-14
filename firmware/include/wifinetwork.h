
#ifndef _GARDEN_AUTOMATION_WIFINETWORK_H
#define _GARDEN_AUTOMATION_WIFINETWORK_H


#include <IPAddress.h>


class WiFiNetwork
{
private:
    String ssid;
    String password;
    ulong connectTimeout;

    uint8_t mac[6];
    IPAddress ip;

public:
    WiFiNetwork(const String ssid, const String password, const ulong connectTimeout);

    uint8_t *getMac();
    IPAddress getIp();

    bool isConnected();
    bool connect();
    bool disconnect();
};


#endif
