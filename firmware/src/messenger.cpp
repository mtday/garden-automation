
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "device.h"
#include "messenger.h"


Messenger::Messenger(
    String deviceId,
    WiFiNetwork *wifiNetwork,
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

    if (functionality & FUNCTIONALITY_SENSOR_TANK_DEPTH) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_TANK_DEPTH);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_TANK_DEPTH);
    }

    if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_TEMPERATURE) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_TEMPERATURE);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_TEMPERATURE);
    }

    if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_HUMIDITY) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_HUMIDITY);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_HUMIDITY);
    }

    if (functionality & FUNCTIONALITY_SENSOR_AMBIENT_PRESSURE) {
        success &= mqttClient.subscribe(TOPIC_SENSOR_AMBIENT_PRESSURE);
    } else {
        success &= mqttClient.unsubscribe(TOPIC_SENSOR_AMBIENT_PRESSURE);
    }

    if (functionality & FUNCTIONALITY_CONTROL_TANK_VALVE) {
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

bool Messenger::publish(String topic, StaticJsonDocument<1024> message)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    char json[1024];
    const size_t length = serializeJson(message, json);
    return mqttClient.publish(topic.c_str(), json, length);
}
