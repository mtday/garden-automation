
#include <Arduino.h>
#include "control/ControlDripValve.hpp"


static ControlDripValve *dripValve;

ControlDripValve::ControlDripValve() {
    currentState = DripValveStateClosed;
}

ControlDripValve *ControlDripValve::get() {
    if (!dripValve) {
        dripValve = new ControlDripValve();
    }
    return dripValve;
}


DripValveState ControlDripValve::getState() {
    return currentState;
}

bool ControlDripValve::open() {
    Serial.println("INFO:  Opening drip valve");
    // TODO
    currentState = DripValveStateOpen;
    return true;
}

bool ControlDripValve::close() {
    Serial.println("INFO:  Closing drip valve");
    // TODO
    currentState = DripValveStateClosed;
    return true;
}
