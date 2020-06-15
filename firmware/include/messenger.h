
#ifndef _GARDEN_AUTOMATION_MESSENGER_H
#define _GARDEN_AUTOMATION_MESSENGER_H


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "ntptime.h"
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
    NTPTime *ntpTime;
    IPAddress *mqttBrokerIP;
    ushort mqttBrokerPort;
    std::function<void(char *, uint8_t *, uint)> messageHandler;

    PubSubClient mqttClient;

protected:
    bool subscribe(const String topic);
    bool unsubscribe(const String topic);
    bool publish(const String topic, const StaticJsonDocument<1024> message);

public:
    Messenger(
        String deviceId,
        WiFiNetwork *wifi,
        NTPTime *ntpTime,
        String mqttBrokerIP,
        ushort mqttBrokerPort,
        std::function<void(char *, uint8_t *, uint)> messageHandler
    );

    bool isConnected();
    bool connect();
    bool disconnect();
    void loop();

    bool handleMessage(Device *device, String topic, StaticJsonDocument<1024> message);

    bool subscribe(const uint8_t functionality);
    bool publishHeartbeat();
    bool publishError(String message);
    bool publishConfigRequest();
    bool publishTankDepth(const float depth);
    bool publishAmbientTemperature(const float temperature);
    bool publishAmbientHumidity(const float humidity);
    bool publishAmbientPressure(const float pressure);
    bool publishTankValveStatus(const boolean status);
};


#endif
