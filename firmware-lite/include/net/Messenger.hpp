
#ifndef _GARDEN_AUTOMATION_NET_MESSENGER_HPP
#define _GARDEN_AUTOMATION_NET_MESSENGER_HPP


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "net/Network.hpp"
#include "net/NTPTime.hpp"
#include "util/Mac.hpp"


// topics
#define TOPIC_HEARTBEAT                  "heartbeat"
#define TOPIC_SENSOR_WEATHER_TEMPERATURE "sensor/weather/temperature"
#define TOPIC_SENSOR_WEATHER_HUMIDITY    "sensor/weather/humidity"
#define TOPIC_SENSOR_WEATHER_PRESSURE    "sensor/weather/pressure"
#define TOPIC_SENSOR_WEATHER_LIGHT       "sensor/weather/light"
#define TOPIC_SENSOR_TANK_VOLUME         "sensor/tank/volume"
#define TOPIC_CONTROL_DRIP_VALVE         "control/drip/valve"
#define TOPIC_STATUS_DRIP_VALVE          "status/drip/valve"


class Messenger {
private:
    PubSubClient mqttClient;
    ulong lastHeartbeat;

protected:
    bool isConnected();
    bool connect();

    bool subscribe(const String topic);
    bool unsubscribe(const String topic);
    bool publish(const String topic, const StaticJsonDocument<1024> message);
    static void callback(char *topic, uint8_t *payload, uint length);

public:
    Messenger();
    static Messenger *get();

    bool setup();
    bool loop();

    bool handleMessage(String topic, StaticJsonDocument<1024> message);
    bool subscribe();

    bool publishHeartbeat();
    bool publishWeatherTemperature(Device *source, const float temperature);
    bool publishWeatherHumidity(Device *source, const float humidity);
    bool publishWeatherPressure(Device *source, const float pressure);
    bool publishWeatherLight(Device *source, const float light);
    bool publishTankVolume(Device *source, const float volume);
    bool publishDripValveStatus(Device *source, const boolean status);
};


#endif
