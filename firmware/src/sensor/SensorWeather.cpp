
#include "sensor/SensorWeather.hpp"


static SensorWeather *sensorWeather;


SensorWeather::SensorWeather() {
    bme = Adafruit_BME280();
}

bool SensorWeather::get(SensorWeather **ref, DeviceType deviceType) {
    if (sensorWeather) {
        *ref = sensorWeather;
        return true;
    }
    if (deviceType != DeviceTypeWeather) {
        *ref = NULL;
        return true;
    }
    sensorWeather = new SensorWeather();
    if (!sensorWeather->setup()) {
        sensorWeather = *ref = NULL;
        return false;
    }
    *ref = sensorWeather;
    return true;
}

bool SensorWeather::setup() {
    Serial.println("INFO:  Initializing Weather sensor");
    if (!TEST_MODE && !bme.begin(BME_SENSOR_ADDRESS)) {
        Serial.println("ERROR: Failed to setup Weather sensor");
        return false;
    }
    return true;
}

float SensorWeather::readTemperature() {
    float temperature = 21.34;
    if (!TEST_MODE) {
        temperature = bme.readTemperature(); // celsius
    }
    Serial.printf("INFO:  Weather sensor read temperature: %f\n", temperature);
    return temperature;
}

float SensorWeather::readHumidity() {
    float humidity = 66.78;
    if (!TEST_MODE) {
        humidity = bme.readHumidity(); // %
    }
    Serial.printf("INFO:  Weather sensor read humidity: %f\n", humidity);
    return humidity;
}

float SensorWeather::readPressure() {
    float pressure = 989.12;
    if (!TEST_MODE) {
        pressure = bme.readPressure(); // pascals
    }
    Serial.printf("INFO:  Weather sensor read pressure: %f\n", pressure);
    return pressure;
}
