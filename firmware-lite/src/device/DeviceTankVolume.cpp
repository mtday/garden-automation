
#include "device/DeviceTankVolume.hpp"


DeviceTankVolume::DeviceTankVolume(Mac mac) : Device(mac)
{
    commSender = new CommSender();
    sensorBME = new SensorBME();
    sensorHCSR = new SensorHCSR(sensorBME, TANK_MAX_VOLUME, TANK_MAX_READING, TANK_MIN_READING);
}

void DeviceTankVolume::setup()
{
    if (!commSender->setup()) {
        Serial.println("Failed to initialize comms");
        while (1);
    }
    if (!sensorBME->setup()) {
        Serial.println("Failed to initialize BME sensor");
        while (1);
    }
    if (!sensorHCSR->setup()) {
        Serial.println("Failed to initialize HCSR sensor");
        while (1);
    }

    const float volume = sensorHCSR->readVolume();

    if (!commSender->sendTankVolume(volume)) {
        Serial.println("Failed to send tank volume data");
    }

    deepSleep(READING_TANK_VOLUME_INTERVAL);
}

void DeviceTankVolume::loop()
{
    // deep sleeping in seupt prevents loop from ever running
}
