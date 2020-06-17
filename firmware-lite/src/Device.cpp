
#include "Device.hpp"


static Device *DEVICE_TANK_1_VOLUME = new Device(MAC_TANK_1_VOLUME, DeviceTypeTankVolume, "Tank 1 Volume");
static Device *DEVICE_TANK_2_VOLUME = new Device(MAC_TANK_2_VOLUME, DeviceTypeTankVolume, "Tank 1 Volume");
static Device *DEVICE_TANK_3_VOLUME = new Device(MAC_TANK_3_VOLUME, DeviceTypeTankVolume, "Tank 1 Volume");
static Device *DEVICE_TANK_4_VOLUME = new Device(MAC_TANK_4_VOLUME, DeviceTypeTankVolume, "Tank 1 Volume");
static Device *DEVICE_DRIP_VALVE    = new Device(MAC_DRIP_VALVE,    DeviceTypeDripValve,  "Drip Valve");
static Device *DEVICE_WEATHER       = new Device(MAC_WEATHER,       DeviceTypeWeather,    "Weather");
static Device *DEVICE_CONTROLLER    = new Device(MAC_CONTROLLER,    DeviceTypeController, "Controller");

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
    if (mac == DEVICE_TANK_1_VOLUME->getMac()) {
        return DEVICE_TANK_1_VOLUME;
    } else if (mac == DEVICE_TANK_2_VOLUME->getMac()) {
        return DEVICE_TANK_2_VOLUME;
    } else if (mac == DEVICE_TANK_3_VOLUME->getMac())
        return DEVICE_TANK_3_VOLUME;
    else if (mac == DEVICE_TANK_4_VOLUME->getMac()) {
        return DEVICE_TANK_4_VOLUME;
    } else if (mac == DEVICE_DRIP_VALVE->getMac()) {
        return DEVICE_DRIP_VALVE;
    } else if (mac == DEVICE_WEATHER->getMac()) {
        return DEVICE_WEATHER;
    } else if (mac == DEVICE_CONTROLLER->getMac()) {
        return DEVICE_CONTROLLER;
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
