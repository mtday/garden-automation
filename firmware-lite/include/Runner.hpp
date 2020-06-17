
#ifndef _GARDEN_AUTOMATION_RUNNER_HPP
#define _GARDEN_AUTOMATION_RUNNER_HPP


class Runner;


#include <stdint.h>
#include "Device.hpp"
#include "net/EspNow.hpp"
#include "net/Messenger.hpp"
#include "net/Network.hpp"
#include "net/NTPTime.hpp"
#include "sensor/SensorBME.hpp"
#include "sensor/SensorGM.hpp"
#include "sensor/SensorHCSR.hpp"
#include "util/Mac.hpp"


class Runner {
private:
    Network *network;
    NTPTime *ntpTime;
    Messenger *messenger;
    EspNow *espNow;
    SensorBME *sensorBME;
    SensorGM *sensorGM;
    SensorHCSR *sensorHCSR;

public:
    Runner();
    static Runner *get();

    bool setup();
    bool loop();

    void deepSleep(const ulong seconds);
    void restart();
};


#endif
