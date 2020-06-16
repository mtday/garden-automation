
#ifndef _GARDEN_AUTOMATION_DEVICE_AUTOMATION_CONTROL_HPP
#define _GARDEN_AUTOMATION_DEVICE_AUTOMATION_CONTROL_HPP


#include "device/Device.hpp"
#include "util/Mac.hpp"


class DeviceAutomationControl : public Device
{
public:
    DeviceAutomationControl(Mac mac);

    void setup();
    void loop();
};


#endif
