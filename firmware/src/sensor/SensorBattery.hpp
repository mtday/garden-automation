
#ifndef _SENSOR_BATTERY_HPP
#define _SENSOR_BATTERY_HPP


#include <stdint.h>
#include "Device.hpp"
#include "net/RestClient.hpp"


class SensorBattery
{
private:
    RestClient *restClient;
    ulong lastBatteryNotification;

protected:
    bool setup();

public:
    SensorBattery(RestClient *restClient);
    static bool get(SensorBattery **ref, DeviceType deviceType, RestClient *restClient);

    bool loop();

    float readVoltage();
};


#endif
