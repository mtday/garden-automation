
#ifndef _GARDEN_AUTOMATION_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_HPP


class Device;


#include <stdint.h>
#include "net/EspNow.hpp"
#include "net/Messenger.hpp"
#include "net/Network.hpp"
#include "net/NTPTime.hpp"
#include "sensor/SensorBME.hpp"
#include "sensor/SensorGM.hpp"
#include "sensor/SensorHCSR.hpp"
#include "util/Mac.hpp"


enum DeviceType
{
    Controller  = 'C',
    Weather     = 'W',
    TankVolume  = 'V',
    TankValve   = 'O',
};


class Device
{
private:
    Mac mac;
    DeviceType type;

    Network *network;
    NTPTime *ntpTime;
    Messenger *messenger;
    EspNow *espNow;
    SensorBME *sensorBME;
    SensorGM *sensorGM;
    SensorHCSR *sensorHCSR;

public:
    Device(Mac mac, DeviceType type);
    static Device *get();

    Mac getMac() const;
    DeviceType getType() const;

    bool setup();
    bool loop();

    void deepSleep(const ulong seconds);
    void restart();
};


#endif
