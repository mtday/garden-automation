
#ifndef _MESSENGER_HPP
#define _MESSENGER_HPP


#include <ArduinoJson.h>
#include <PubSubClient.h>


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
    bool publishBatteryVoltage(Device *source, const float voltage);
    bool publishWeatherTemperature(Device *source, const float temperature);
    bool publishWeatherHumidity(Device *source, const float humidity);
    bool publishWeatherPressure(Device *source, const float pressure);
    bool publishWeatherLight(Device *source, const float light);
    bool publishTankDistance(Device *source, const float distance);
    bool publishDripValveStatus(Device *source, const boolean status);
};


#endif
