
#ifndef _GARDEN_AUTOMATION_MESSENGER_H
#define _GARDEN_AUTOMATION_MESSENGER_H


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifinetwork.h"


class Messenger
{
private:
    const char *deviceId;
    WiFiNetwork *wifiNetwork;
    IPAddress *mqttBrokerIP;
    ushort mqttBrokerPort;
    std::function<void(char*, uint8_t*, uint)> messageHandler;

    PubSubClient mqttClient;

protected:
    bool send(const char *topic, StaticJsonDocument<1024> doc);

public:
    Messenger(
        const char *deviceId,
        WiFiNetwork *wifi,
        const char *mqttBrokerIP,
        const ushort mqttBrokerPort,
        std::function<void(char*, uint8_t*, uint)> messageHandler
    );

    bool isConnected();
    bool connect();
    bool disconnect();
    void loop();

    bool sendConfigRequest();
};


#endif
