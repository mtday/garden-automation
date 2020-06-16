
#ifndef _GARDEN_AUTOMATION_SENSOR_BME_SENSOR_HPP
#define _GARDEN_AUTOMATION_SENSOR_BME_SENSOR_HPP


#include <Adafruit_BME280.h>


class BmeSensor
{
private:
    Adafruit_BME280 bme;

public:
    BmeSensor();

    bool setup();

    float readTemperature();
    float readHumidity();
    float readPressure();
};


#endif
