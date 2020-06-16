
#include "device/TankVolumeDevice.hpp"


TankVolumeDevice::TankVolumeDevice(Mac mac) : Device(mac)
{
    bmeSensor = new BmeSensor();
    hcsrSensor = new HcsrSensor(bmeSensor, TANK_MAX_VOLUME, TANK_MAX_READING, TANK_MIN_READING);
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
