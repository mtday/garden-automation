
#ifndef _GARDEN_AUTOMATION_BMESENSOR_H
#define _GARDEN_AUTOMATION_BMESENSOR_H


#include <Adafruit_BME280.h>

#define BME_SENSOR_ADDRESS 0x76


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
