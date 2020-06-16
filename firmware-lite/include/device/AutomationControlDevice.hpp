
#ifndef _GARDEN_AUTOMATION_DEVICE_AUTOMATION_CONTROL_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_AUTOMATION_CONTROL_DEVICE_HPP


#include "device/Device.hpp"
#include "util/Mac.hpp"


class AutomationControlDevice : public Device
{
public:
    AutomationControlDevice(Mac mac);

    void setup();
    void loop();
};


#endif
