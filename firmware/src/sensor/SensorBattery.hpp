
#ifndef _SENSOR_BATTERY_HPP
#define _SENSOR_BATTERY_HPP


#include <stdint.h>
#include "Device.hpp"


class SensorBattery {
private:
    ulong lastBatteryNotification;

protected:
    bool setup();

public:
    SensorBattery();
    static bool get(SensorBattery **ref, DeviceType deviceType);

    bool loop();

    float readVoltage();
};


#endif
