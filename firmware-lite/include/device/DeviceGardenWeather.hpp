
#ifndef _GARDEN_AUTOMATION_DEVICE_GARDEN_WEATHER_HPP
#define _GARDEN_AUTOMATION_DEVICE_GARDEN_WEATHER_HPP


#include "comm/CommSender.hpp"
#include "device/Device.hpp"
#include "sensor/SensorBME.hpp"
#include "sensor/SensorGM.hpp"
#include "util/Mac.hpp"


class DeviceGardenWeather : public Device
{
private:
    CommSender *commSender;
    SensorBME *sensorBME;
    SensorGM *sensorGM;

public:
    DeviceGardenWeather(Mac mac);

    void setup();
    void loop();
};


#endif
