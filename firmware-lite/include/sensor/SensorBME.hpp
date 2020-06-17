
#ifndef _GARDEN_AUTOMATION_SENSOR_BME_HPP
#define _GARDEN_AUTOMATION_SENSOR_BME_HPP


#include <Adafruit_BME280.h>


class SensorBME
{
private:
    Adafruit_BME280 bme;

public:
    SensorBME();
    static SensorBME *get();

    bool setup();

    float readTemperature();
    float readHumidity();
    float readPressure();
};


#endif
