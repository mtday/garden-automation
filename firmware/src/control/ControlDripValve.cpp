
#include <Arduino.h>
#include "control/ControlDripValve.hpp"


static ControlDripValve *dripValve;

ControlDripValve::ControlDripValve() {
    currentState = DripValveStateClosed;
}

bool ControlDripValve::get(ControlDripValve **ref, DeviceType deviceType) {
    if (dripValve) {
        *ref = dripValve;
        return true;
    }
    if (deviceType != DeviceTypeDripValve) {
        *ref = NULL;
        return true;
    }
    dripValve = new ControlDripValve();
    if (!dripValve->setup()) {
        dripValve = *ref = NULL;
        return false;
    }
    *ref = dripValve;
    return true;
}

bool ControlDripValve::setup() {
    Serial.println("INFO:  Initializing drip valve");
    // TODO
    return true;
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
