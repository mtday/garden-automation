
#ifndef _GARDEN_AUTOMATION_SENSOR_HCSR_HPP
#define _GARDEN_AUTOMATION_SENSOR_HCSR_HPP


#include <stdint.h>
#include "sensor/SensorBME.hpp"


class SensorHCSR
{
private:
    SensorBME *sensorBME;
    float maxVolume;
    float maxReading;
    float minReading;

public:
    SensorHCSR(
        SensorBME *sensorBME,
        const float maxVolume,
        const float maxReading,
        const float minReading
    );

    bool setup();
    float readVolume();
};


#endif
