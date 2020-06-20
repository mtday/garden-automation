
#include <Arduino.h>
#include "net/EspNow.hpp"
#include "sensor/SensorBattery.hpp"


static SensorBattery *sensorBattery;


SensorBattery::SensorBattery(EspNow *espNow) {
    SensorBattery::lastBatteryNotification = 0;
}

bool SensorBattery::get(SensorBattery **ref, DeviceType deviceType, EspNow *espNow) {
    if (sensorBattery) {
        *ref = sensorBattery;
        return true;
    }
    if (deviceType == DeviceTypeController) {
        *ref = NULL;
        return true;
    }
    sensorBattery = new SensorBattery(espNow);
    if (!sensorBattery->setup()) {
        sensorBattery = *ref = NULL;
        return false;
    }
    *ref = sensorBattery;
    return true;
}

bool SensorBattery::setup() {
    Serial.println("INFO:  Initializing Battery sensor");
    if (!TEST_MODE) {
        // TODO
    }
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
    float voltage = 3.7;
    if (!TEST_MODE) {
        // TODO
    }
    Serial.printf("INFO:  Battery sensor read voltage: %f\n", voltage);
    return voltage;
}
