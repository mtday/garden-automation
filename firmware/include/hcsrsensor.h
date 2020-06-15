
#ifndef _GARDEN_AUTOMATION_HCSRSENSOR_H
#define _GARDEN_AUTOMATION_HCSRSENSOR_H


#include <stdint.h>
#include "bmesensor.h"

#define HCSR_TRIGGER_PIN 11
#define HCSR_ECHO_PIN 12


class HcsrSensor
{
private:
    uint8_t triggerPin;
    uint8_t echoPin;
    BmeSensor *bmeSensor;
    uint8_t functionality;
    float maxReading;
    float minReading;

public:
    HcsrSensor(BmeSensor *bmeSensor);

    bool setup();
    void configure(const uint8_t functonality, const float maxReading, const float minReading);
    float readLevel();
};


#endif
