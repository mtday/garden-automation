
#ifndef _GARDEN_AUTOMATION_SENSOR_GM_HPP
#define _GARDEN_AUTOMATION_SENSOR_GM_HPP


#include <Arduino.h>


class SensorGM {
public:
    SensorGM();
    static SensorGM *get();

    bool setup();

    float readLight();
};


#endif
