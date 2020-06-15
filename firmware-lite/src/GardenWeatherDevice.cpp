
#include "GardenWeatherDevice.hpp"


GardenWeatherDevice::GardenWeatherDevice(Mac mac) : Device(mac)
{
    bmeSensor = new BmeSensor();
    gmSensor = new GmSensor();
}

void GardenWeatherDevice::setup()
{
    if (!bmeSensor->setup()) {
        Serial.println("Failed to initialize BME sensor");
        while (1);
    }
    if (!gmSensor->setup()) {
        Serial.println("Failed to initialize GM sensor");
        while (1);
    }
}

void GardenWeatherDevice::loop()
{
}
