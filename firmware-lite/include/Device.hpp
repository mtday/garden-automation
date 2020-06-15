
#ifndef _GARDEN_AUTOMATION_DEVICE_H
#define _GARDEN_AUTOMATION_DEVICE_H


#include <stdint.h>
#include "Mac.hpp"

using namespace std;


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
