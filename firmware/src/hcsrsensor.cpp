
#include <Arduino.h>
#include "functionality.h"
#include "hcsrsensor.h"


HcsrSensor::HcsrSensor(BmeSensor *bmeSensor) {
    HcsrSensor::bmeSensor = bmeSensor;

    HcsrSensor::functionality = 0;
    // the maximum reading of the sensor (when the tank is empty)
    HcsrSensor::maxReading = NAN;
    // the minimum reading of the sensor (when the tank is full)
    HcsrSensor::minReading = NAN;
}

bool HcsrSensor::setup()
{
    pinMode(HCSR_TRIGGER_PIN, OUTPUT);
    pinMode(HCSR_ECHO_PIN, INPUT);
    return true;
}

void HcsrSensor::configure(const uint8_t functonality, const float maxReading, const float minReading)
{
    HcsrSensor::functionality = functionality;
    HcsrSensor::maxReading = maxReading;
    HcsrSensor::minReading = minReading;
}

float HcsrSensor::readLevel()
{
    if (maxReading == NAN || minReading == NAN) {
        return NAN;
    }

    // The sensor is triggered by a HIGH pulse of 10 or more microseconds. Give a short LOW pulse
    // beforehand to ensure a clean HIGH pulse:
    digitalWrite(HCSR_TRIGGER_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(HCSR_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HCSR_TRIGGER_PIN, LOW);
 
    // Read the signal from the sensor: a HIGH pulse whose duration is the time (in microseconds)
    // from the sending of the ping to the reception of its echo off of an object.
    pinMode(HCSR_ECHO_PIN, INPUT);
    const ulong duration = pulseIn(HCSR_ECHO_PIN, HIGH);

    bool hasTemp = hasFunctionality(functionality, FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE);
    bool hasHum = hasFunctionality(functionality, FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY);

    float speedOfSound = 0.0343;
    if (hasTemp && hasHum) {
        speedOfSound = 0.03314 + (0.606 * bmeSensor->readTemperature()) + (0.0124 * bmeSensor->readHumidity());
    } else if (hasTemp) {
        speedOfSound = 0.03314 + (0.606 * bmeSensor->readTemperature());
    } else if (hasHum) {
        speedOfSound += (0.0124 * bmeSensor->readHumidity());
    }

    // divide by 2 since duraiton is round trip, distance in centimeters to the surface of the water
    const float distance = (duration / 2.0) * speedOfSound;

    // examples:
    // 100 - ((7 - 4) / (10 - 4) * 100) = 100 - (3 / 6 * 100) = 100 - 50   = 50.0%
    // 100 - ((9 - 4) / (10 - 4) * 100) = 100 - (5 / 6 * 100) = 100 - 83.3 = 16.7%
    return 100.0 - ((distance - minReading) / (maxReading - minReading) * 100.0);
}
