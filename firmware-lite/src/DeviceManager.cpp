
#include <WiFi.h>
#include "DeviceManager.hpp"
#include "GardenWeatherDevice.hpp"
#include "TankValveDevice.hpp"
#include "TankVolumeDevice.hpp"


DeviceManager::DeviceManager()
{
    Mac mac;
    if (mac == TANK_1_VOLUME_DEVICE || mac == TANK_2_VOLUME_DEVICE
            || mac == TANK_3_VOLUME_DEVICE || mac == TANK_4_VOLUME_DEVICE) {
        device = new TankVolumeDevice(mac);
    } else if (mac == TANK_VALVE_DEVICE) {
        device = new TankValveDevice(mac);
    } else if (mac == GARDEN_WEATHER_DEVICE) {
        device = new GardenWeatherDevice(mac);
    } else {
        Serial.printf("ERROR: Unrecognized device mac address: %s\n", mac.c_str());
        while (true);
    }
}

Device *DeviceManager::getDevice()
{
    return device;
}

