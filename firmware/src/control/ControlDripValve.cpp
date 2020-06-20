
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
    if (!TEST_MODE && deviceType != DeviceTypeDripValve) {
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
    if (!TEST_MODE) {
        // TODO
    }
    return true;
}

DripValveState ControlDripValve::getState() {
    return currentState;
}

bool ControlDripValve::open() {
    Serial.println("INFO:  Opening drip valve");
    if (!TEST_MODE) {
        // TODO
    }
    currentState = DripValveStateOpen;
    return true;
}

bool ControlDripValve::close() {
    Serial.println("INFO:  Closing drip valve");
    if (!TEST_MODE) {
        // TODO
    }
    currentState = DripValveStateClosed;
    return true;
}
