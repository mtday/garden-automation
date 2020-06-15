
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "device.h"
#include "functionality.h"
#include "messenger.h"


Messenger::Messenger(
    String deviceId,
    WiFiNetwork *wifiNetwork,
    NTPTime *ntpTime,
    String mqttBrokerIP,
    ushort mqttBrokerPort,
    std::function<void(char *, uint8_t *, uint)> messageHandler
) {
    IPAddress brokerIP = IPAddress();
    brokerIP.fromString(mqttBrokerIP);
    EthernetClient ethernetClient;

    Messenger::deviceId = deviceId;
    Messenger::wifiNetwork = wifiNetwork;
    Messenger::mqttBrokerIP = &brokerIP;
    Messenger::mqttBrokerPort = mqttBrokerPort;
    Messenger::messageHandler = messageHandler;
    Messenger::mqttClient = PubSubClient(ethernetClient);
    Messenger::mqttClient.setCallback(messageHandler);
    Messenger::mqttClient.setBufferSize(1024);
}

bool Messenger::isConnected()
{
    return mqttClient.connected();
}

bool Messenger::connect()
{
    if (isConnected() || mqttClient.connect(deviceId.c_str())) {
        return true;
    }
    return false;
}

bool Messenger::disconnect()
{
    if (isConnected()) {
        mqttClient.disconnect();
        return true;
    }
    return false;
}

void Messenger::loop()
{
    if (isConnected() || connect()) {
        mqttClient.loop();
    }
}

bool Messenger::subscribe(const uint8_t functionality)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    bool success = true;
    success &= mqttClient.subscribe((String(TOPIC_CONFIG) + "/" + deviceId).c_str());

    if (hasFunctionality(functionality, FUNCTIONALITY_SENSOR_TANK_LEVEL)) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_TANK_LEVEL);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_TANK_LEVEL);
    }

    if (hasFunctionality(functionality, FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE)) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_TEMPERATURE);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_TEMPERATURE);
    }

    if (hasFunctionality(functionality, FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY)) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_HUMIDITY);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_HUMIDITY);
    }

    if (hasFunctionality(functionality, FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE)) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_PRESSURE);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_PRESSURE);
    }

    if (hasFunctionality(functionality, FUNCTIONALITY_CONTROL_TANK_VALVE)) {
        success &= mqttClient.subscribe(TOPIC_CONTROL_TANK_VALVE);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_CONTROL_TANK_VALVE);
    }

    return success;
}

bool Messenger::unsubscribe(String topic)
{
    if (!isConnected() && !connect()) {
        return false;
    }
    return mqttClient.unsubscribe(topic.c_str());
}

bool Messenger::handleMessage(Device *device, String topic, StaticJsonDocument<1024> message)
{
    if (topic == String(TOPIC_CONFIG) + "/" + deviceId) {
        const uint8_t functionality = message["functionality"];
        const float maxReading = hasFunctionality(functionality, FUNCTIONALITY_SENSOR_TANK_LEVEL)
                ? message["maxReading"]
                : NAN;
        const float minReading = hasFunctionality(functionality, FUNCTIONALITY_SENSOR_TANK_LEVEL)
                ? message["minReading"]
                : NAN;
        return device->configure(functionality, maxReading, minReading);
    } else if (hasFunctionality(device->getFunctionality(), FUNCTIONALITY_SENSOR_TANK_LEVEL)
            && topic == TOPIC_SENSOR_TANK_LEVEL) {
        return device->readTankLevel();
    } else if (hasFunctionality(device->getFunctionality(), FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE)
            && topic == TOPIC_SENSOR_AMBIENT_TEMPERATURE) {
        return device->readAmbientTemperature();
    } else if (hasFunctionality(device->getFunctionality(), FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY)
            && topic == TOPIC_SENSOR_AMBIENT_HUMIDITY) {
        return device->readAmbientHumidity();
    } else if (hasFunctionality(device->getFunctionality(), FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE)
            && topic == TOPIC_SENSOR_AMBIENT_PRESSURE) {
        return device->readAmbientPressure();
    } else if (hasFunctionality(device->getFunctionality(), FUNCTIONALITY_CONTROL_TANK_VALVE)
            && topic == TOPIC_CONTROL_TANK_VALVE) {
        return device->controlTankValve(message["status"]);
    } else {
        publishError(String("Received message on unsupported topic: ") + topic);
        return false;
    }
}

bool Messenger::publish(String topic, StaticJsonDocument<1024> message)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    char json[1024];
    const size_t length = serializeJson(message, json);
    Serial.printf("Sending: %s => %s\n", topic.c_str(), json);
    return mqttClient.publish(topic.c_str(), json, length);
}

bool Messenger::publishHeartbeat()
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    return publish(String(TOPIC_HEARTBEAT) + "/" + deviceId, message);
}

bool Messenger::publishError(String error)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["error"] = error;
    return publish(String(TOPIC_ERROR) + "/" + deviceId, message);
}

bool Messenger::publishConfigRequest()
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    return publish(String(TOPIC_CONFIG), message);
}

bool Messenger::publishTankLevel(const float level)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["level"] = level;
    message["unit"] = "percent";
    return publish(String(TOPIC_SENSOR_TANK_LEVEL) + "/" + deviceId, message);
}

bool Messenger::publishAmbientTemperature(const float temperature)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["temperature"] = temperature;
    message["unit"] = "celsius";
    return publish(String(TOPIC_SENSOR_AMBIENT_TEMPERATURE) + "/" + deviceId, message);
}

bool Messenger::publishAmbientHumidity(const float humidity)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["humidity"] = humidity;
    message["unit"] = "percent";
    return publish(String(TOPIC_SENSOR_AMBIENT_HUMIDITY) + "/" + deviceId, message);
}

bool Messenger::publishAmbientPressure(const float pressure)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["pressure"] = pressure;
    message["unit"] = "pascals";
    return publish(String(TOPIC_SENSOR_AMBIENT_PRESSURE) + "/" + deviceId, message);
}

bool Messenger::publishTankValveStatus(const boolean status)
{
    StaticJsonDocument<1024> message;
    message["device"] = deviceId;
    message["timestamp"] = ntpTime->now();
    message["status"] = status;
    return publish(String(TOPIC_CONTROL_TANK_VALVE) + "/" + deviceId, message);
}
