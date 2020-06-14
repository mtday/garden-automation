
#ifndef _GARDEN_AUTOMATION_DEVICE_H
#define _GARDEN_AUTOMATION_DEVICE_H


#include "messenger.h"
#include "ntptime.h"
#include "wifinetwork.h"


// device functionality
#define FUNCTIONALITY_SENSOR_TANK_DEPTH          (1 << 0) // 1
#define FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE (1 << 2) // 2
#define FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY    (1 << 3) // 4
#define FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE    (1 << 4) // 8
#define FUNCTIONALITY_CONTROL_TANK_VALVE         (1 << 5) // 16


class Device
{
private:
    String id;
    uint8_t functionality;

    WiFiNetwork *wifiNetwork;
    Messenger *messenger;
    NTPTime *ntpTime;

    ulong lastHeartbeat;

public:
    Device(
        const String wifissid,
        const String wifiPassword,
        const ulong wifiConnectTimeout,
        const String mqttBrokerIP,
        const ushort mqttBrokerPort,
        std::function<void(char *, uint8_t *, uint)> messageHandler
    );

    void setup();
    void loop();

    bool notifyError(String message);
    bool heartbeat();

    bool requestConfig();
    bool configure(StaticJsonDocument<1024> message);

    bool handleMessage(String topic, StaticJsonDocument<1024> message);

    // sensor readings
    bool readTankDepth(StaticJsonDocument<1024> message);
    bool readAmbientTemperature(StaticJsonDocument<1024> message);
    bool readAmbientHumidity(StaticJsonDocument<1024> message);
    bool readAmbientPressure(StaticJsonDocument<1024> message);

    // control actions
    bool controlTankValve(StaticJsonDocument<1024> message);
};


#endif
