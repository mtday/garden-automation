
#include <Arduino.h>
#include "sensor/SensorBattery.hpp"


static SensorBattery *sensorBattery;


SensorBattery::SensorBattery(RestClient *restClient)
{
    SensorBattery::restClient = restClient;
    SensorBattery::lastBatteryNotification = 0;
}

bool SensorBattery::get(SensorBattery **ref, DeviceType deviceType, RestClient *restClient)
{
    if (sensorBattery)
    {
        *ref = sensorBattery;
        return true;
    }
    sensorBattery = new SensorBattery(restClient);
    if (!sensorBattery->setup())
    {
        sensorBattery = *ref = NULL;
        return false;
    }
    *ref = sensorBattery;
    return true;
}

bool SensorBattery::setup()
{
    Serial.println("INFO:  Initializing Battery sensor");
    if (!TEST_MODE)
    {
        // TODO
    }
    return true;
}

bool SensorBattery::loop()
{
    const ulong now = millis();
    const ulong interval = TEST_MODE ? READING_BATTERY_INTERVAL_TEST_MODE : READING_BATTERY_INTERVAL;
    if (now - lastBatteryNotification > interval)
    {
        lastBatteryNotification = now;
        const float voltage = readVoltage();
        if (!restClient->publishBatteryVoltage(Device::get(), voltage))
        {
            return false;
        }
    }
    return true;
}

float SensorBattery::readVoltage()
{
    float voltage = 3.7;
    if (!TEST_MODE)
    {
        // TODO
    }
    Serial.printf("INFO:  Battery sensor read voltage: %f\n", voltage);
    return voltage;
}
