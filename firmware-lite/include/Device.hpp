
#ifndef _GARDEN_AUTOMATION_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_HPP


#include <stdint.h>
#include "comm/EspNow.hpp"
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

    EspNow *espNow;
    SensorBME *sensorBME;
    SensorGM *sensorGM;
    SensorHCSR *sensorHCSR;

public:
    static Device *getDevice();

    Device(Mac mac, DeviceType type);

    Mac getMac() const;
    DeviceType getType() const;

    bool setup();
    bool loop();

    void deepSleep(const ulong seconds);

    void weather(Mac source, const float temperature, const float humidity, const float pressure, const float light);
    void tankVolume(Mac source, const float volume);
    void tankValve(Mac source, const bool status);
};


#endif
