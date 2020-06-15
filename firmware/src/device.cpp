
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "device.h"


Device::Device(
    const String wifissid,
    const String wifiPassword,
    const ulong wifiConnectTimeout,
    const String mqttBrokerIP,
    const ushort mqttBrokerPort,
    std::function<void(char *, uint8_t *, uint)> messageHandler
) {
    uint8_t macAddress[6];
    WiFi.macAddress(macAddress);
    char deviceId[5];
    snprintf(deviceId, 5, "%02x%02x", macAddress[4], macAddress[5]);

    id = deviceId;
    functionality = 0; // no capabilities are available until configuration happens
    lastHeartbeat = 0;

    wifiNetwork = new WiFiNetwork(wifissid, wifiPassword, wifiConnectTimeout);
    ntpTime = new NTPTime();
    messenger = new Messenger(id, wifiNetwork, ntpTime, mqttBrokerIP, mqttBrokerPort, messageHandler);
    bmeSensor = new BmeSensor();
}

void Device::setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.printf("Device ID: %s\n", id.c_str());

    if (!wifiNetwork->connect()) {
        Serial.println("Failed to connect to network, restarting in 5 seconds");
        delay(5000);
        esp_restart();
    }
    if (!messenger->connect()) {
        Serial.println("Failed to connect to MQTT broker, restarting in 5 seconds");
        delay(5000);
        esp_restart();
    }

    ntpTime->setup();
    Serial.printf("Current time: %ld\n", ntpTime->now());

    if (!messenger->publishConfigRequest()) {
        Serial.println("Failed to request device configuration, restarting in 5 seconds");
        delay(5000);
        esp_restart();
    }
}

void Device::loop()
{
    Serial.println("LOOP");
    messenger->loop();
    delay(5000);

    ulong now = millis();
    if (now - lastHeartbeat > HEARTBEAT_INTERVAL && messenger->publishHeartbeat()) {
        lastHeartbeat = now;
    }
}

bool Device::configure(const uint8_t functionality)
{
    Device::functionality = functionality;

    // setup the bme sensor when necessary
    if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE
            || functionality & FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY
            || functionality & FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE) {
        if (!bmeSensor->setup()) {
            messenger->publishError("Failed to initialize sensor.");
            return false;
        }
    }

    // update subscriptions based on the functionality configured for this device
    return messenger->subscribe(functionality);
}

uint8_t Device::getFunctionality()
{
    return functionality;
}

bool Device::hasFunctionality(const uint8_t functionality)
{
    return Device::functionality & functionality;
}

bool Device::handleMessage(String topic, StaticJsonDocument<1024> message)
{
    return messenger->handleMessage(this, topic, message);
}

bool Device::readTankDepth()
{
    // TODO: actually read the tank depth
    const float depth = 33.5; // in centimeters
    if (depth == NAN) {
        messenger->publishError("Failed to read tank depth from sensor.");
        return false;
    }
    return messenger->publishTankDepth(depth);
}

bool Device::readAmbientTemperature()
{
    const float temperature = bmeSensor->readTemperature(); // in Celsius
    if (temperature == NAN) {
        messenger->publishError("Failed to read temperature from sensor.");
        return false;
    }
    return messenger->publishAmbientTemperature(temperature);
}

bool Device::readAmbientHumidity()
{
    // TODO: actually read the humidity
    const float humidity = bmeSensor->readHumidity();
    if (humidity == NAN) {
        messenger->publishError("Failed to read humidity from sensor.");
        return false;
    }
    return messenger->publishAmbientTemperature(humidity);
}

bool Device::readAmbientPressure()
{
    // TODO: actually read the pressure
    const float pressure = bmeSensor->readPressure();
    if (pressure == NAN) {
        messenger->publishError("Failed to read pressure from sensor.");
        return false;
    }
    return messenger->publishAmbientPressure(pressure);
}

bool Device::controlTankValve(const bool status)
{
    // TODO: actually control the valve
    const bool updatedStatus = status;
    if (false) { // TODO
        messenger->publishError("Failed to control tank valve.");
        return false;
    }
    return messenger->publishTankValveStatus(updatedStatus);
}
