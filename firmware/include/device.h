
#ifndef _GARDEN_AUTOMATION_DEVICE_H
#define _GARDEN_AUTOMATION_DEVICE_H


class Device;


#include "bmesensor.h"
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
    BmeSensor *bmeSensor;

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

    bool configure(const uint8_t functionality);
    uint8_t getFunctionality();
    bool hasFunctionality(const uint8_t functionality);

    bool handleMessage(String topic, StaticJsonDocument<1024> message);

    // perform sensor readings
    bool readTankDepth();
    bool readAmbientTemperature();
    bool readAmbientHumidity();
    bool readAmbientPressure();

    // perform control actions
    bool controlTankValve(bool status);
};


#endif
