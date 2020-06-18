
#ifndef _SENSOR_BATTERY_HPP
#define _SENSOR_BATTERY_HPP


class SensorBattery {
public:
    SensorBattery();
    static SensorBattery *get();

    bool setup();

    float readVoltage();
};


#endif
