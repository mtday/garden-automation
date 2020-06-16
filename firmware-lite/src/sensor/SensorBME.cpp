
#include "sensor/SensorBME.hpp"


SensorBME::SensorBME()
{
    bme = Adafruit_BME280();
}

bool SensorBME::setup()
{
    return bme.begin(BME_SENSOR_ADDRESS);
}

float SensorBME::readTemperature()
{
    return bme.readTemperature(); // celsius
}

float SensorBME::readHumidity()
{
    return bme.readHumidity(); // %
}

float SensorBME::readPressure()
{
    return bme.readPressure(); // pascals
}
