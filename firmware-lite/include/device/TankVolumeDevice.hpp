
#ifndef _GARDEN_AUTOMATION_DEVICE_TANK_VOLUME_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_TANK_VOLUME_DEVICE_HPP


#include "device/Device.hpp"
#include "sensor/BmeSensor.hpp"
#include "sensor/HcsrSensor.hpp"
#include "util/Mac.hpp"


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
