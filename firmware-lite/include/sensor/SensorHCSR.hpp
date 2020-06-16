
#ifndef _GARDEN_AUTOMATION_SENSOR_HCSR_HPP
#define _GARDEN_AUTOMATION_SENSOR_HCSR_HPP


#include <stdint.h>
#include "sensor/SensorBME.hpp"


class SensorHCSR
{
private:
    SensorBME *sensorBME;

public:
    SensorHCSR(SensorBME *sensorBME);

    bool setup();
    float readVolume();
};


#endif
