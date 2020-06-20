
#ifndef _MESSENGER_HPP
#define _MESSENGER_HPP


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <stdint.h>
#include "Device.hpp"
#include "net/EspNow.hpp"
#include "control/ControlDripValve.hpp"


class Messenger {
private:
    EspNow *espNow;

    PubSubClient mqttClient;
    ulong lastHeartbeat;

protected:
    bool isConnected();
    bool connect();

    bool setup();

    bool subscribe(const String topic);
    bool unsubscribe(const String topic);
    bool publish(const String topic, const StaticJsonDocument<1024> message);
    static void callback(char *topic, uint8_t *payload, uint length);

public:
    Messenger(EspNow *espNow);
    static bool get(Messenger **ref, DeviceType deviceType, EspNow *espNow);

    bool loop();

    bool handleMessage(String topic, StaticJsonDocument<1024> message);
    bool subscribe();

    bool publishHeartbeat();
    bool publishBatteryVoltage(Device *source, const float voltage);
    bool publishWeatherTemperature(Device *source, const float temperature);
    bool publishWeatherHumidity(Device *source, const float humidity);
    bool publishWeatherPressure(Device *source, const float pressure);
    bool publishWeatherLight(Device *source, const float light);
    bool publishTankDistance(Device *source, const uint8_t tank, const float distance);
    bool publishDripValveState(Device *source, const DripValveState state);
};


#endif