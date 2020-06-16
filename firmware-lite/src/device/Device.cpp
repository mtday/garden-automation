
#include "device/Device.hpp"
#include "device/DeviceAutomationControl.hpp"
#include "device/DeviceGardenWeather.hpp"
#include "device/DeviceTankValve.hpp"
#include "device/DeviceTankVolume.hpp"


Device *Device::createDevice()
{
    Mac mac;
    if (mac == DEVICE_TANK_1_VOLUME ||
        mac == DEVICE_TANK_2_VOLUME ||
        mac == DEVICE_TANK_3_VOLUME ||
        mac == DEVICE_TANK_4_VOLUME) {
        return new DeviceTankVolume(mac);
    } else if (mac == DEVICE_TANK_VALVE) {
        return new DeviceTankValve(mac);
    } else if (mac == DEVICE_GARDEN_WEATHER) {
        return new DeviceGardenWeather(mac);
    } else if (mac == DEVICE_AUTOMATION_CONTROL) {
        return new DeviceAutomationControl(mac);
    } else {
        Serial.printf("ERROR: Unrecognized device mac address: %s\n", mac.c_str());
        return NULL;
    }
}

Device::Device(Mac mac)
{
    Device::mac = mac;
    Serial.printf("MAC: %s\n", mac.c_str());
}

Mac Device::getMac() const
{
    return mac;
}

void Device::deepSleep(const ulong seconds)
{
    const ulong microseconds = seconds * 1000 * 1000;
    esp_sleep_enable_timer_wakeup(microseconds);

    Serial.printf("Sleeping for %ld seconds\n", seconds);
    Serial.flush(); 
    esp_deep_sleep_start();
}
