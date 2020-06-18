
#ifndef _RUNNER_HPP
#define _RUNNER_HPP


class Runner;


#include <stdint.h>
#include "net/EspNow.hpp"
#include "net/Messenger.hpp"
#include "net/Network.hpp"
#include "net/NetworkTime.hpp"
#include "sensor/SensorBattery.hpp"
#include "sensor/SensorDistance.hpp"
#include "sensor/SensorLight.hpp"
#include "sensor/SensorWeather.hpp"


class Runner {
private:
    EspNow *espNow;
    Network *network;
    NetworkTime *networkTime;
    Messenger *messenger;
    SensorBattery *sensorBattery;
    SensorDistance *sensorDistance;
    SensorLight *sensorLight;
    SensorWeather *sensorWeather;

    ulong lastBatteryNotification;

public:
    Runner();
    static Runner *get();

    bool setup();
    bool loop();

    void deepSleep(const ulong seconds);
    void restart();
};


#endif
