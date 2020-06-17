
#ifndef _GARDEN_AUTOMATION_SENSOR_BATTERY_HPP
#define _GARDEN_AUTOMATION_SENSOR_BATTERY_HPP


#include <Arduino.h>


class SensorBattery {
public:
    SensorBattery();
    static SensorBattery *get();

    bool setup();

    float readVoltage();
};


#endif
