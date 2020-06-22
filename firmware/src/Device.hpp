
#ifndef _DEVICE_HPP
#define _DEVICE_HPP


#include <stdint.h>
#include "util/Mac.hpp"


enum DeviceType
{
    DeviceTypeWeather     = 'W',
    DeviceTypeTankGroup   = 'T',
    DeviceTypeDripValve   = 'D',
};


class Device
{
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
