
#ifndef _SENSOR_LIGHT_HPP
#define _SENSOR_LIGHT_HPP


class SensorLight {
public:
    SensorLight();
    static SensorLight *get();

    bool setup();

    float readLight();
};


#endif
