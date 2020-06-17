
#ifndef _GARDEN_AUTOMATION_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_HPP


#include <stdint.h>
#include "util/Mac.hpp"


enum DeviceType {
    DeviceTypeController  = 'C',
    DeviceTypeWeather     = 'W',
    DeviceTypeTankVolume  = 'V',
    DeviceTypeDripValve   = 'D',
};


class Device {
private:
    Mac mac;
    DeviceType type;
    String name;

public:
    Device(Mac mac, DeviceType type, String name);
    static Device *get();
    static Device *get(Mac mac);

    Mac getMac() const;
    DeviceType getType() const;
    String getName() const;

    bool operator==(const Device& device) const;

    const char *c_str();
};


#endif
