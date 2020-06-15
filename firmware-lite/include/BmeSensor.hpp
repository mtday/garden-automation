
#ifndef _GARDEN_AUTOMATION_BME_SENSOR_H
#define _GARDEN_AUTOMATION_BME_SENSOR_H


#include <Adafruit_BME280.h>

using namespace std;

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
