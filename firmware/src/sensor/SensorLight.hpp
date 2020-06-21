
#ifndef _SENSOR_LIGHT_HPP
#define _SENSOR_LIGHT_HPP


#include "Device.hpp"


class SensorLight
{
protected:
    bool setup();

public:
    SensorLight();
    static bool get(SensorLight **ref, DeviceType deviceType);

    float readLight();
};


#endif
