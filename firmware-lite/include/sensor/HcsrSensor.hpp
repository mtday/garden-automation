
#ifndef _GARDEN_AUTOMATION_SENSOR_HCSR_SENSOR_HPP
#define _GARDEN_AUTOMATION_SENSOR_HCSR_SENSOR_HPP


#include <stdint.h>
#include "sensor/BmeSensor.hpp"


class HcsrSensor
{
private:
    BmeSensor *bmeSensor;
    float maxVolume;
    float maxReading;
    float minReading;

public:
    HcsrSensor(
        BmeSensor *bmeSensor,
        const float maxVolume,
        const float maxReading,
        const float minReading
    );

    bool setup();
    float readVolume();
};


#endif
