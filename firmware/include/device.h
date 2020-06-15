
#ifndef _GARDEN_AUTOMATION_DEVICE_H
#define _GARDEN_AUTOMATION_DEVICE_H


class Device;


#include "bmesensor.h"
#include "hcsrsensor.h"
#include "messenger.h"
#include "ntptime.h"
#include "wifinetwork.h"


class Device
{
private:
    String id;
    uint8_t functionality;

    WiFiNetwork *wifiNetwork;
    Messenger *messenger;
    NTPTime *ntpTime;
    BmeSensor *bmeSensor;
    HcsrSensor *hcsrSensor;

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

    bool configure(const uint8_t functionality, const float maxReading, const float minReading);
    uint8_t getFunctionality();

    bool handleMessage(String topic, StaticJsonDocument<1024> message);

    // perform sensor readings
    bool readTankLevel();
    bool readAmbientTemperature();
    bool readAmbientHumidity();
    bool readAmbientPressure();

    // perform control actions
    bool controlTankValve(bool status);
};


#endif
