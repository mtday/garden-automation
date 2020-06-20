
#ifndef _SENSOR_BATTERY_HPP
#define _SENSOR_BATTERY_HPP


#include <stdint.h>
#include "Device.hpp"


class SensorBattery {
private:
    Messenger *messenger;
    ulong lastBatteryNotification;

protected:
    bool setup();

public:
    SensorBattery(Messenger *messenger);
    static bool get(SensorBattery **ref, DeviceType deviceType, Messenger *messenger);

    bool loop();

    float readVoltage();
};


#endif
