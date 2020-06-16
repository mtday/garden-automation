
#include <WiFi.h>
#include "device/AutomationControlDevice.hpp"
#include "device/DeviceManager.hpp"
#include "device/GardenWeatherDevice.hpp"
#include "device/TankValveDevice.hpp"
#include "device/TankVolumeDevice.hpp"


DeviceManager::DeviceManager()
{
    Mac mac;
    if (mac == DEVICE_TANK_1_VOLUME ||
        mac == DEVICE_TANK_2_VOLUME ||
        mac == DEVICE_TANK_3_VOLUME ||
        mac == DEVICE_TANK_4_VOLUME) {
        device = new TankVolumeDevice(mac);
    } else if (mac == DEVICE_TANK_VALVE) {
        device = new TankValveDevice(mac);
    } else if (mac == DEVICE_GARDEN_WEATHER) {
        device = new GardenWeatherDevice(mac);
    } else if (mac == DEVICE_AUTOMATION_CONTROL) {
        device = new AutomationControlDevice(mac);
    } else {
        Serial.printf("ERROR: Unrecognized device mac address: %s\n", mac.c_str());
        while (true);
    }
}

Device *DeviceManager::getDevice()
{
    return device;
}

