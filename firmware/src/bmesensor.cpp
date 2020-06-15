
#include "bmesensor.h"


BmeSensor::BmeSensor()
{
    bme = Adafruit_BME280();
}

bool BmeSensor::setup()
{
    return bme.begin(BME_SENSOR_ADDRESS);
}

float BmeSensor::readTemperature()
{
    return bme.readTemperature(); // celsius
}

float BmeSensor::readHumidity()
{
    return bme.readHumidity(); // %
}

float BmeSensor::readPressure()
{
    return bme.readPressure(); // pascals
}
