
#ifndef _GARDEN_AUTOMATION_TANK_VOLUME_DEVICE_H
#define _GARDEN_AUTOMATION_TANK_VOLUME_DEVICE_H


#include "BmeSensor.hpp"
#include "Device.hpp"
#include "HcsrSensor.hpp"
#include "Mac.hpp"


class TankVolumeDevice : public Device
{
private:
    BmeSensor *bmeSensor;
    HcsrSensor *hcsrSensor;

public:
    TankVolumeDevice(Mac mac);

    void setup();
    void loop();
};


#endif
