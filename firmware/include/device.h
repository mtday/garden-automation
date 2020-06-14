
#ifndef _GARDEN_AUTOMATION_DEVICE_H
#define _GARDEN_AUTOMATION_DEVICE_H


#include "messenger.h"
#include "ntptime.h"
#include "wifinetwork.h"


typedef enum {
    DEVICE_TYPE_UNKNOWN,
    DEVICE_TYPE_TANK_SENSOR,
    DEVICE_TYPE_TANK_VALVE_CONTROLLER
} device_type_t;


class Device
{
private:
    const char *id;
    device_type_t type;

    WiFiNetwork *wifiNetwork;
    Messenger *messenger;
    NTPTime *ntpTime;

public:
    Device(
        const char *wifissid,
        const char *wifiPassword,
        const ulong wifiConnectTimeout,
        const char *mqttBrokerIP,
        const ushort mqttBrokerPort,
        std::function<void(char *, uint8_t *, uint)> messageHandler
    );

    void setup();
    void loop();
    void handleMessage(char *topic, uint8_t *payload, uint length);
};


#endif
