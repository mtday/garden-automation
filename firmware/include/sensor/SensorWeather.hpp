
#ifndef _SENSOR_WEATHER_HPP
#define _SENSOR_WEATHER_HPP


#include <Adafruit_BME280.h>


class SensorWeather {
private:
    Adafruit_BME280 bme;

public:
    SensorWeather();
    static SensorWeather *get();

    bool setup();

    float readTemperature();
    float readHumidity();
    float readPressure();
};


#endif
