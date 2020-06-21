
#ifndef _CONTROL_DRIP_VALVE_HPP
#define _CONTROL_DRIP_VALVE_HPP


#include "Device.hpp"


enum DripValveState
{
    DripValveStateClosed = 0,
    DripValveStateOpen   = 1
};


class ControlDripValve
{
private:
    DripValveState currentState;

protected:
    bool setup();

public:
    ControlDripValve();
    static bool get(ControlDripValve **ref, DeviceType deviceType);

    DripValveState getState();
    bool open();
    bool close();
};


#endif
