
#ifndef _GARDEN_AUTOMATION_DEVICE_TANK_VOLUME_HPP
#define _GARDEN_AUTOMATION_DEVICE_TANK_VOLUME_HPP


#include "comm/CommSender.hpp"
#include "device/Device.hpp"
#include "sensor/SensorBME.hpp"
#include "sensor/SensorHCSR.hpp"
#include "util/Mac.hpp"


class DeviceTankVolume : public Device
{
private:
    CommSender *commSender;
    SensorBME *sensorBME;
    SensorHCSR *sensorHCSR;

public:
    DeviceTankVolume(Mac mac);

    void setup();
    void loop();
};


#endif
