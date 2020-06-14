
#ifndef _GARDEN_AUTOMATION_MESSENGER_H
#define _GARDEN_AUTOMATION_MESSENGER_H


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifinetwork.h"


// topics
#define TOPIC_ERROR                      "error"
#define TOPIC_CONFIG                     "config"
#define TOPIC_HEARTBEAT                  "heartbeat"
#define TOPIC_SENSOR_TANK_DEPTH          "sensor/tank/depth"
#define TOPIC_SENSOR_AMBIENT_TEMPERATURE "sensor/ambient/temperature"
#define TOPIC_SENSOR_AMBIENT_HUMIDITY    "sensor/ambient/humidity"
#define TOPIC_SENSOR_AMBIENT_PRESSURE    "sensor/ambient/pressure"
#define TOPIC_CONTROL_TANK_VALVE         "control/tank/valve"


class Messenger
{
private:
    String deviceId;
    WiFiNetwork *wifiNetwork;
    IPAddress *mqttBrokerIP;
    ushort mqttBrokerPort;
    std::function<void(char *, uint8_t *, uint)> messageHandler;

    PubSubClient mqttClient;

public:
    Messenger(
        String deviceId,
        WiFiNetwork *wifi,
        String mqttBrokerIP,
        ushort mqttBrokerPort,
        std::function<void(char *, uint8_t *, uint)> messageHandler
    );

    bool isConnected();
    bool connect();
    bool disconnect();
    void loop();

    bool subscribe(const uint8_t deviceType);
    bool subscribe(const String topic);
    bool unsubscribe(const String topic);
    bool publish(const String topic, const StaticJsonDocument<1024> doc);
};


#endif
