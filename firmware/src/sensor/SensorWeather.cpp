
#include "sensor/SensorWeather.hpp"


static SensorWeather *sensorWeather;


SensorWeather::SensorWeather() {
    bme = Adafruit_BME280();
}

SensorWeather *SensorWeather::get() {
    if (!sensorWeather) {
        sensorWeather = new SensorWeather();
    }
    return sensorWeather;
}

bool SensorWeather::setup() {
    Serial.println("INFO:  Initializing Weather sensor");
    if (!bme.begin(BME_SENSOR_ADDRESS)) {
        Serial.println("Failed to setup Weather sensor");
        return false;
    }
    return true;
}

float SensorWeather::readTemperature() {
    const float temperature = bme.readTemperature(); // celsius
    Serial.printf("INFO:  Weather sensor read temperature: %f\n", temperature);
    return temperature;
}

float SensorWeather::readHumidity() {
    const float humidity = bme.readHumidity(); // %
    Serial.printf("INFO:  Weather sensor read humidity: %f\n", humidity);
    return humidity;
}

float SensorWeather::readPressure() {
    const float pressure = bme.readPressure(); // pascals
    Serial.printf("INFO:  Weather sensor read pressure: %f\n", pressure);
    return pressure;
}
