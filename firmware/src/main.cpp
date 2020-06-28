
#include <Arduino.h>
#include <WiFi.h>
#include <stdint.h>

#include "Device.hpp"
#include "control/ControlDripValve.hpp"
#include "net/Network.hpp"
#include "net/NetworkTime.hpp"
#include "net/RestClient.hpp"
#include "sensor/SensorBattery.hpp"
#include "sensor/SensorDistance.hpp"
#include "sensor/SensorLight.hpp"
#include "sensor/SensorWeather.hpp"


static Device *device;

static Network *network;
static NetworkTime *networkTime;
static RestClient *restClient;
static SensorBattery *sensorBattery;
static SensorDistance *sensorDistance;
static SensorLight *sensorLight;
static SensorWeather *sensorWeather;
static ControlDripValve *controlDripValve;


void deepSleep(const ulong seconds)
{
    const ulong microseconds = seconds * 1000 * 1000;
    esp_sleep_enable_timer_wakeup(microseconds);

    Serial.printf("INFO:  Deep sleep for %ld seconds\n", seconds);
    Serial.flush(); 
    esp_deep_sleep_start();
}


void restart()
{
    Serial.flush(); 
    delay(10000);
    esp_restart();
}


void setup()
{
    Serial.begin(SERIAL_BAUD);

    device = Device::get();
    if (!device)
    {
        restart();
    }

    Serial.println();
    Serial.printf("INFO:  Initializing device %s\n", device->c_str());

    bool initialized =
        Network::get(&network) &&
        NetworkTime::get(&networkTime) &&
        RestClient::get(&restClient, networkTime) &&
        SensorBattery::get(&sensorBattery, device->getType(), restClient) &&
        SensorWeather::get(&sensorWeather, device->getType()) &&
        SensorLight::get(&sensorLight, device->getType()) &&
        SensorDistance::get(&sensorDistance, device->getType(), sensorWeather) &&
        ControlDripValve::get(&controlDripValve, device->getType());
    if (!initialized)
    {
        Serial.println("ERROR: Restarting due to setup failure");
        restart();
    }

    switch (device->getType())
    {
        case DeviceTypeWeather:
        {
            Serial.println("INFO:  Performing weather readings");
            const float voltage = sensorBattery->readVoltage();
            const float temperature = sensorWeather->readTemperature();
            const float humidity = sensorWeather->readHumidity();
            const float pressure = sensorWeather->readPressure();
            const float light = sensorLight->readLight();

            bool success =
                restClient->publishBatteryVoltage(device, voltage) &&
                restClient->publishWeatherTemperature(device, temperature) &&
                restClient->publishWeatherHumidity(device, humidity) &&
                restClient->publishWeatherPressure(device, pressure) &&
                restClient->publishWeatherLight(device, light);
            if (success)
            {
                restart();
            }
            deepSleep(TEST_MODE ? WEATHER_SLEEP_PERIOD_TEST_MODE : WEATHER_SLEEP_PERIOD);
        }

        case DeviceTypeTankGroup:
        {
            Serial.println("INFO:  Performing tank readings");
            const float voltage = sensorBattery->readVoltage();
            if (!restClient->publishBatteryVoltage(device, voltage))
            {
                restart();
            }
            for (int tank = 0; tank < NUM_TANKS; tank++)
            {
                const float distance = sensorDistance->readDistance(tank);
                if (!restClient->publishTankDistance(device, tank + 1, distance))
                {
                    restart();
                }
            }
            deepSleep(TEST_MODE ? TANK_SLEEP_PERIOD_TEST_MODE : TANK_SLEEP_PERIOD);
        }

        case DeviceTypeDripValve:
        {
            Serial.println("INFO:  Drip valve initialized, waiting for control requests");
        }

        default:
            Serial.printf("ERROR: Unsupported device type: %c\n", device->getType());
            restart();
    }
}

void loop()
{
    bool success =
        network->loop() &&
        (!sensorBattery || sensorBattery->loop());
    if (!success)
    {
        restart();
    }
}
