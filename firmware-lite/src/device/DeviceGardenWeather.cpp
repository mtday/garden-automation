
#include "device/DeviceGardenWeather.hpp"


DeviceGardenWeather::DeviceGardenWeather(Mac mac) : Device(mac)
{
    commSender = new CommSender();
    sensorBME = new SensorBME();
    sensorGM = new SensorGM();
}

void DeviceGardenWeather::setup()
{
    if (!commSender->setup()) {
        Serial.println("Failed to initialize comms");
        while (1);
    }
    if (!sensorBME->setup()) {
        Serial.println("Failed to initialize BME sensor");
        while (1);
    }
    if (!sensorGM->setup()) {
        Serial.println("Failed to initialize GM sensor");
        while (1);
    }

    const float temperature = sensorBME->readTemperature();
    const float humidity = sensorBME->readHumidity();
    const float presssure = sensorBME->readPressure();
    const float light = sensorGM->readLight();

    if (!commSender->sendWeather(temperature, humidity, presssure, light)) {
        Serial.println("Failed to send weather data");
    }

    deepSleep(READING_WEATHER_INTERVAL);
}

void DeviceGardenWeather::loop()
{
    // deep sleeping in seupt prevents loop from ever running
}
