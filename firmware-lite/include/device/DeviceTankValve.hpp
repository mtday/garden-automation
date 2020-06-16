
#ifndef _GARDEN_AUTOMATION_DEVICE_TANK_VALVE_HPP
#define _GARDEN_AUTOMATION_DEVICE_TANK_VALVE_HPP


#include "device/Device.hpp"
#include "util/Mac.hpp"


class DeviceTankValve : public Device
{
public:
    DeviceTankValve(Mac mac);

    void setup();
    void loop();
};


#endif
