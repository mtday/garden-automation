
#include <Arduino.h>
#include "net/Messenger.hpp"
#include "sensor/SensorBattery.hpp"


static SensorBattery *sensorBattery;


SensorBattery::SensorBattery(Messenger *messenger) {
    SensorBattery::messenger = messenger;
    SensorBattery::lastBatteryNotification = 0;
}

bool SensorBattery::get(SensorBattery **ref, DeviceType deviceType, Messenger *messenger) {
    if (sensorBattery) {
        *ref = sensorBattery;
        return true;
    }
    sensorBattery = new SensorBattery(messenger);
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
        if (!messenger->publishBatteryVoltage(Device::get(), voltage)) {
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
