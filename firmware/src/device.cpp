
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
    sprintf(deviceId, "%02x%02x", macAddress[4], macAddress[5]);

    id = deviceId;
    functionality = 0; // no capabilities are available until configuration happens
    lastHeartbeat = 0;

    wifiNetwork = new WiFiNetwork(wifissid, wifiPassword, wifiConnectTimeout);
    messenger = new Messenger(id, wifiNetwork, mqttBrokerIP, mqttBrokerPort, messageHandler);
    ntpTime = new NTPTime();
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

    if (!requestConfig()) {
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
    if (now - lastHeartbeat > HEARTBEAT_INTERVAL && heartbeat()) {
        lastHeartbeat = now;
    }
}

bool Device::notifyError(String message)
{
    Serial.println(message.c_str());

    StaticJsonDocument<1024> notification;
    notification["device"] = id;
    notification["timestamp"] = ntpTime->now();
    notification["message"] = message.c_str();
    return messenger->publish(TOPIC_ERROR, notification);
}

bool Device::heartbeat()
{
    StaticJsonDocument<1024> heartbeat;
    heartbeat["device"] = id;
    heartbeat["timestamp"] = ntpTime->now();
    return messenger->publish(TOPIC_HEARTBEAT, heartbeat);
}

bool Device::requestConfig()
{
    StaticJsonDocument<1024> request;
    request["device"] = id;
    request["timestamp"] = ntpTime->now();
    return messenger->publish(TOPIC_CONFIG, request);
}

bool Device::configure(StaticJsonDocument<1024> message)
{
    functionality = message["functionality"];

    // update subscriptions based on the functionality configured for this device
    return messenger->subscribe(functionality);
}

bool Device::handleMessage(String topic, StaticJsonDocument<1024> message)
{
    if (topic == String(TOPIC_CONFIG) + "/" + id) {
        return configure(message);
    } else if (functionality & FUNCTIONALITY_SENSOR_TANK_DEPTH
            && topic == TOPIC_SENSOR_TANK_DEPTH) {
        return readTankDepth(message);
    } else if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE
            && topic == TOPIC_SENSOR_AMBIENT_TEMPERATURE) {
        return readAmbientTemperature(message);
    } else if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY
            && topic == TOPIC_SENSOR_AMBIENT_HUMIDITY) {
        return readAmbientHumidity(message);
    } else if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE
            && topic == TOPIC_SENSOR_AMBIENT_PRESSURE) {
        return readAmbientPressure(message);
    } else if (functionality & FUNCTIONALITY_CONTROL_TANK_VALVE
            && topic == TOPIC_CONTROL_TANK_VALVE) {
        return controlTankValve(message);
    } else {
        notifyError(String("Received message on unsupported topic: ") + topic);
        return false;
    }
}

bool Device::readTankDepth(StaticJsonDocument<1024> message)
{
    // TODO: actually read the tank depth
    const float depth = 33.5; // in centimeters

    StaticJsonDocument<1024> reading;
    reading["device"] = id;
    reading["timestamp"] = ntpTime->now();
    reading["depth"] = depth;
    return messenger->publish(String(TOPIC_SENSOR_TANK_DEPTH) + "/" + id, reading);
}

bool Device::readAmbientTemperature(StaticJsonDocument<1024> message)
{
    // TODO: actually read the temperature
    const float temperature = 79.5; // in Fahrenheit

    StaticJsonDocument<1024> reading;
    reading["device"] = id;
    reading["timestamp"] = ntpTime->now();
    reading["temperature"] = temperature;
    return messenger->publish(String(TOPIC_SENSOR_AMBIENT_TEMPERATURE) + "/" + id, reading);
}

bool Device::readAmbientHumidity(StaticJsonDocument<1024> message)
{
    // TODO: actually read the humidity
    const float humidity = 79.5;

    StaticJsonDocument<1024> reading;
    reading["device"] = id;
    reading["timestamp"] = ntpTime->now();
    reading["humidity"] = humidity;
    return messenger->publish(String(TOPIC_SENSOR_AMBIENT_HUMIDITY) + "/" + id, reading);
}

bool Device::readAmbientPressure(StaticJsonDocument<1024> message)
{
    // TODO: actually read the pressure
    const float pressure = 1102.5;

    StaticJsonDocument<1024> reading;
    reading["device"] = id;
    reading["timestamp"] = ntpTime->now();
    reading["pressure"] = pressure;
    return messenger->publish(String(TOPIC_SENSOR_AMBIENT_PRESSURE) + "/" + id, reading);
}


bool Device::controlTankValve(StaticJsonDocument<1024> message)
{
    // TODO: actually control the valve
    const bool state = true;

    StaticJsonDocument<1024> control;
    control["device"] = id;
    control["timestamp"] = ntpTime->now();
    control["state"] = state;
    return messenger->publish(String(TOPIC_CONTROL_TANK_VALVE) + "/" + id, control);
}
