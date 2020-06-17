
#include "sensor/SensorBME.hpp"


static SensorBME *sensorBME;


SensorBME::SensorBME() {
    bme = Adafruit_BME280();
}

SensorBME *SensorBME::get() {
    if (!sensorBME) {
        sensorBME = new SensorBME();
    }
    return sensorBME;
}

bool SensorBME::setup() {
    Serial.println("INFO:  Initializing BME sensor");
    if (!bme.begin(BME_SENSOR_ADDRESS)) {
        Serial.println("Failed to setup BME sensor");
        return false;
    }
    return true;
}

float SensorBME::readTemperature() {
    const float temperature = bme.readTemperature(); // celsius
    Serial.printf("INFO:  BME sensor read temperature: %f\n", temperature);
    return temperature;
}

float SensorBME::readHumidity() {
    const float humidity = bme.readHumidity(); // %
    Serial.printf("INFO:  BME sensor read humidity: %f\n", humidity);
    return humidity;
}

float SensorBME::readPressure() {
    const float pressure = bme.readPressure(); // pascals
    Serial.printf("INFO:  BME sensor read pressure: %f\n", pressure);
    return pressure;
}
