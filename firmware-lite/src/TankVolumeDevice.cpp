
#include "TankVolumeDevice.hpp"


TankVolumeDevice::TankVolumeDevice(Mac mac) : Device(mac)
{
    bmeSensor = new BmeSensor();
}

void TankVolumeDevice::setup()
{
    if (!bmeSensor->setup()) {
        Serial.println("Failed to initialize BME sensor");
        while (1);
    }
}

void TankVolumeDevice::loop()
{
}
