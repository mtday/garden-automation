
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "messenger.h"


Messenger::Messenger(
    const char *deviceId,
    WiFiNetwork *wifiNetwork,
    const char *mqttBrokerIP,
    const ushort mqttBrokerPort,
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
    Messenger::mqttClient.subscribe("#");
}

bool Messenger::isConnected()
{
    return mqttClient.connected();
}

bool Messenger::connect()
{
    if (isConnected() || mqttClient.connect(deviceId)) {
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
    mqttClient.loop();
}

bool Messenger::send(const char *topic, StaticJsonDocument<1024> doc)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    char buffer[1024];
    size_t length = serializeJson(doc, buffer);
    return mqttClient.publish(topic, buffer, length);
}

bool Messenger::sendConfigRequest()
{
    StaticJsonDocument<1024> doc;
    doc["device"] = deviceId;
    return send("config/request", doc);
}
