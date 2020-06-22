
#ifndef _SENSOR_DISTANCE_HPP
#define _SENSOR_DISTANCE_HPP


#include "Device.hpp"
#include "sensor/SensorWeather.hpp"


class SensorDistance
{
private:
    SensorWeather *sensorWeather;

protected:
    bool setup();

public:
    SensorDistance(SensorWeather *sensorWeather);
    static bool get(SensorDistance **ref, DeviceType deviceType, SensorWeather *sensorWeather);

    float readDistance(const uint8_t tank);
};


#endif
