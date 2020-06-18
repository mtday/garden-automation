
#ifndef _CONTROL_DRIP_VALVE_HPP
#define _CONTROL_DRIP_VALVE_HPP


enum DripValveState {
    DripValveStateClosed = 0,
    DripValveStateOpen   = 1
};

class ControlDripValve {
private:
    DripValveState currentState;

public:
    ControlDripValve();
    static ControlDripValve *get();

    DripValveState getState();
    bool open();
    bool close();
};


#endif
