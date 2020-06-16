
#ifndef _GARDEN_AUTOMATION_DEVICE_DEVICE_MANAGER_HPP
#define _GARDEN_AUTOMATION_DEVICE_DEVICE_MANAGER_HPP


#include <stdint.h>
#include "device/Device.hpp"
#include "util/Mac.hpp"


class DeviceManager
{
private:
    Device *device;

public:
    DeviceManager();

    Device *getDevice();
};


#endif
