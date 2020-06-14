
#ifndef _GARDEN_AUTOMATION_WIFINETWORK_H
#define _GARDEN_AUTOMATION_WIFINETWORK_H


#include <IPAddress.h>


class WiFiNetwork
{
private:
    const char *ssid;
    const char *password;
    ulong connectTimeout;

    uint8_t mac[6];
    IPAddress ip;

public:
    WiFiNetwork(const char *ssid, const char *password, const ulong connectTimeout);

    uint8_t *getMac();
    IPAddress getIp();

    bool isConnected();
    bool connect();
    bool disconnect();
};


#endif
