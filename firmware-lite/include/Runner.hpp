
#ifndef _GARDEN_AUTOMATION_RUNNER_HPP
#define _GARDEN_AUTOMATION_RUNNER_HPP


class Runner;


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


class Runner
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
    Runner(Mac mac, DeviceType type);
    static Runner *get();

    Mac getMac() const;
    DeviceType getType() const;

    bool setup();
    bool loop();

    void deepSleep(const ulong seconds);
    void restart();
};


#endif
