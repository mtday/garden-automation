
#ifndef _SENSOR_WEATHER_HPP
#define _SENSOR_WEATHER_HPP


#include <Adafruit_BME280.h>
#include "Device.hpp"


class SensorWeather
{
private:
    Adafruit_BME280 bme;

protected:
    bool setup();

public:
    SensorWeather();
    static bool get(SensorWeather **ref, DeviceType deviceType);

    float readTemperature();
    float readHumidity();
    float readPressure();
};


#endif
