
#include <Arduino.h>
#include "sensor/SensorLight.hpp"


static SensorLight *sensorLight;


SensorLight::SensorLight() {
}

bool SensorLight::get(SensorLight **ref, DeviceType deviceType) {
    if (sensorLight) {
        *ref = sensorLight;
        return true;
    }
    if (!TEST_MODE && deviceType != DeviceTypeWeather) {
        *ref = NULL;
        return true;
    }
    sensorLight = new SensorLight();
    if (!sensorLight->setup()) {
        sensorLight = *ref = NULL;
        return false;
    }
    *ref = sensorLight;
    return true;
}

bool SensorLight::setup() {
    Serial.println("INFO:  Initializing Light sensor");
    if (!TEST_MODE) {
        // TODO
    }
    return true;
}

float SensorLight::readLight() {
    float light = 23.45;
    if (!TEST_MODE) {
        // TODO
    }
    Serial.printf("INFO:  Light sensor read light: %f\n", light);
    return light;
}
