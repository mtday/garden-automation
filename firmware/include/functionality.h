
#ifndef _GARDEN_AUTOMATION_FUNCTIONALITY_H
#define _GARDEN_AUTOMATION_FUNCTIONALITY_H


#include <stdint.h>


// device functionality
#define FUNCTIONALITY_SENSOR_TANK_LEVEL          (1 << 0) // 1
#define FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE (1 << 2) // 2
#define FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY    (1 << 3) // 4
#define FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE    (1 << 4) // 8
#define FUNCTIONALITY_CONTROL_TANK_VALVE         (1 << 5) // 16


static bool hasFunctionality(const uint8_t functionality, const uint8_t fn)
{
    return functionality & fn;
}


#endif
