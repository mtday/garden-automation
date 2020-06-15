
#ifndef _GARDEN_AUTOMATION_TANK_VALVE_DEVICE_H
#define _GARDEN_AUTOMATION_TANK_VALVE_DEVICE_H


#include "Device.hpp"
#include "Mac.hpp"

using namespace std;


class TankValveDevice : public Device
{
public:
    TankValveDevice(Mac mac);

    void setup();
    void loop();
};


#endif
