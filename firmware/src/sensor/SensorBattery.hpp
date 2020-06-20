
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
    SensorBattery(EspNow *espNow);
    static bool get(SensorBattery **ref, DeviceType deviceType, EspNow *espNow);

    bool loop();

    float readVoltage();
};


#endif
