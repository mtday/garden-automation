
#ifndef _GARDEN_AUTOMATION_DEVICE_DEVICE_HPP
#define _GARDEN_AUTOMATION_DEVICE_DEVICE_HPP


#include <stdint.h>
#include "util/Mac.hpp"


class Device
{
private:
    Mac mac;

public:
    Device(Mac mac);

    Mac getMac() const;

    virtual void setup() = 0;
    virtual void loop() = 0;
};


#endif
