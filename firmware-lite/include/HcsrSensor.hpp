
#ifndef _GARDEN_AUTOMATION_HCSR_SENSOR_H
#define _GARDEN_AUTOMATION_HCSR_SENSOR_H


#include <stdint.h>
#include "BmeSensor.hpp"

using namespace std;

#define HCSR_TRIGGER_PIN 11
#define HCSR_ECHO_PIN 12


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
