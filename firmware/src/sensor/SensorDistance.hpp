
#ifndef _SENSOR_DISTANCE_HPP
#define _SENSOR_DISTANCE_HPP


class SensorDistance {
public:
    SensorDistance();
    static SensorDistance *get();

    bool setup();
    float readDistance();
};


#endif
