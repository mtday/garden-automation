
#include <Arduino.h>
#include "sensor/SensorBattery.hpp"


static SensorBattery *sensorBattery;


SensorBattery::SensorBattery() {
}

SensorBattery *SensorBattery::get() {
    if (!sensorBattery) {
        sensorBattery = new SensorBattery();
    }
    return sensorBattery;
}

bool SensorBattery::setup() {
    Serial.println("INFO:  Initializing Battery sensor");
    return true;
}

float SensorBattery::readVoltage() {
    const float voltage = 0.0;
    Serial.printf("INFO:  Battery sensor read voltage: %f\n", voltage);
    return voltage;
}
