
#include <Arduino.h>
#include "sensor/SensorDistance.hpp"
#include "sensor/SensorWeather.hpp"

static SensorDistance *sensorDistance;


SensorDistance::SensorDistance() {
}

SensorDistance *SensorDistance::get() {
    if (!sensorDistance) {
        sensorDistance = new SensorDistance();
    }
    return sensorDistance;
}

bool SensorDistance::setup() {
    Serial.println("INFO:  Initializing Distance sensor");
    pinMode(HCSR_TRIGGER_PIN, OUTPUT);
    pinMode(HCSR_ECHO_PIN, INPUT);
    return true;
}

float SensorDistance::readDistance() {
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

    float temp = SensorWeather::get()->readTemperature();
    float hum = SensorWeather::get()->readHumidity();
    float speedOfSound = 0.03314 + (0.606 * temp) + (0.0124 * hum);

    // divide by 2 since duration is round trip, distance in centimeters to the surface of the water
    const float distance = (duration / 2.0) * speedOfSound;

    // to calculate how full the tank is:
    // float percentFull = 100.0 - ((distance - TANK_MIN_READING) / (TANK_MAX_READING - TANK_MIN_READING) * 100.0);
    // float volume = TANK_MAX_VOLUME * percentFull; // gallons
    // examples:
    // 100 - ((7 - 4) / (10 - 4) * 100) = 100 - (3 / 6 * 100) = 100 - 50   = 50.0%
    // 100 - ((9 - 4) / (10 - 4) * 100) = 100 - (5 / 6 * 100) = 100 - 83.3 = 16.7%

    Serial.printf("INFO:  Distance sensor read distance: %f\n", distance);
    return distance;
}
