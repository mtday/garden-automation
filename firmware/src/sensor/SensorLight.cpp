
#include <Arduino.h>
#include "sensor/SensorLight.hpp"


static SensorLight *sensorLight;


SensorLight::SensorLight() {
}

SensorLight *SensorLight::get() {
    if (!sensorLight) {
        sensorLight = new SensorLight();
    }
    return sensorLight;
}

bool SensorLight::setup() {
    Serial.println("INFO:  Initializing Light sensor");
    return true;
}

float SensorLight::readLight() {
    const float light = 0.0;
    Serial.printf("INFO:  Light sensor read light: %f\n", light);
    return light;
}
