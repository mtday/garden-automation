
#ifndef _GARDEN_AUTOMATION_DEVICE_GARDEN_WEATHER_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_GARDEN_WEATHER_DEVICE_HPP


#include "device/Device.hpp"
#include "sensor/BmeSensor.hpp"
#include "sensor/GmSensor.hpp"
#include "util/Mac.hpp"


class GardenWeatherDevice : public Device
{
private:
    BmeSensor *bmeSensor;
    GmSensor *gmSensor;

public:
    GardenWeatherDevice(Mac mac);

    void setup();
    void loop();
};


#endif
