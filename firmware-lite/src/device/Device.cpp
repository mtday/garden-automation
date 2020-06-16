
#include "device/Device.hpp"


Device::Device(Mac mac)
{
    Device::mac = mac;
    Serial.printf("MAC: %s\n", mac.c_str());
}

Mac Device::getMac() const
{
    return mac;
}
