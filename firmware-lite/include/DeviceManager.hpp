
#ifndef _GARDEN_AUTOMATION_DEVICE_MANAGER_H
#define _GARDEN_AUTOMATION_DEVICE_MANAGER_H


#include <stdint.h>
#include "Device.hpp"
#include "Mac.hpp"

using namespace std;


#define TANK_1_VOLUME_DEVICE   Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x01)
#define TANK_2_VOLUME_DEVICE   Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x02)
#define TANK_3_VOLUME_DEVICE   Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x03)
#define TANK_4_VOLUME_DEVICE   Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x04)
#define TANK_VALVE_DEVICE      Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x05)
#define GARDEN_WEATHER_DEVICE  Mac(0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x06)



class DeviceManager
{
private:
    Device *device;

public:
    DeviceManager();

    Device *getDevice();
};


#endif
