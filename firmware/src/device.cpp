
#include <Arduino.h>
#include "Device.hpp"


static Device *DEVICE_WEATHER       = new Device(MAC_WEATHER,      DeviceTypeWeather,    "Weather");
static Device *DEVICE_TANK_GROUP_A  = new Device(MAC_TANK_GROUP_A, DeviceTypeTankGroup,  "Tank Group A");
static Device *DEVICE_TANK_GROUP_B  = new Device(MAC_TANK_GROUP_B, DeviceTypeTankGroup,  "Tank Group B");
static Device *DEVICE_DRIP_VALVE    = new Device(MAC_DRIP_VALVE,   DeviceTypeDripValve,  "Drip Valve");

static Device *device;


Device::Device(Mac mac, DeviceType type, String name) {
    Device::mac = mac;
    Device::type = type;
    Device::name = name;
}

Device *Device::get() {
    if (!device) {
        device = Device::get(Mac());
    }
    return device;
}

Device *Device::get(Mac mac) {
    if (mac == DEVICE_WEATHER->getMac()) {
        return DEVICE_WEATHER;
    } else if (mac == DEVICE_TANK_GROUP_A->getMac()) {
        return DEVICE_TANK_GROUP_A;
    } else if (mac == DEVICE_TANK_GROUP_B->getMac()) {
        return DEVICE_TANK_GROUP_B;
    } else if (mac == DEVICE_DRIP_VALVE->getMac()) {
        return DEVICE_DRIP_VALVE;
    } else {
        Serial.printf("ERROR: This device has an unrecognized Mac address: %s\n", mac.c_str());
        return NULL;
    }
}

Mac Device::getMac() const {
    return mac;
}

DeviceType Device::getType() const {
    return type;
}

String Device::getName() const {
    return name;
}

bool Device::operator==(const Device& device) const {
    return mac == device.mac;
}

const char *Device::c_str() {
    return (name + " (" + mac.c_str() + ")").c_str();
}
