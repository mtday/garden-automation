
#include <Arduino.h>
#include "net/EspNow.hpp"
#include "sensor/SensorBattery.hpp"


static SensorBattery *sensorBattery;


SensorBattery::SensorBattery() {
    SensorBattery::lastBatteryNotification = 0;
}

bool SensorBattery::get(SensorBattery **ref, DeviceType deviceType) {
    if (sensorBattery) {
        *ref = sensorBattery;
        return true;
    }
    if (deviceType == DeviceTypeController) {
        *ref = NULL;
        return true;
    }
    sensorBattery = new SensorBattery();
    if (!sensorBattery->setup()) {
        sensorBattery = *ref = NULL;
        return false;
    }
    *ref = sensorBattery;
    return true;
}

bool SensorBattery::setup() {
    Serial.println("INFO:  Initializing Battery sensor");
    return true;
}

bool SensorBattery::loop() {
    const ulong now = millis();
    if (now - lastBatteryNotification > READING_BATTERY_INTERVAL) {
        lastBatteryNotification = now;
        const float voltage = readVoltage();

        EspNow *espNow;
        EspNow::get(&espNow);

        if (!espNow->sendBattery(voltage)) {
            return false;
        }
    }
    return true;
}

float SensorBattery::readVoltage() {
    const float voltage = 0.0;
    Serial.printf("INFO:  Battery sensor read voltage: %f\n", voltage);
    return voltage;
}
