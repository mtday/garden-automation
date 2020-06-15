
#ifndef _GARDEN_AUTOMATION_GARDEN_WEATHER_DEVICE_H
#define _GARDEN_AUTOMATION_GARDEN_WEATHER_DEVICE_H


#include "BmeSensor.hpp"
#include "Device.hpp"
#include "GmSensor.hpp"
#include "Mac.hpp"

using namespace std;


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
