
#ifndef _GARDEN_AUTOMATION_DEVICE_TANK_VALVE_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_TANK_VALVE_DEVICE_HPP


#include "device/Device.hpp"
#include "util/Mac.hpp"


class TankValveDevice : public Device
{
public:
    TankValveDevice(Mac mac);

    void setup();
    void loop();
};


#endif
