
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "net/EspNow.hpp"
#include "net/Messenger.hpp"


static Messenger *messenger;


Messenger::Messenger() {
    IPAddress brokerIP = IPAddress();
    brokerIP.fromString(MQTT_BROKER_IP);
    EthernetClient ethernetClient;

    Messenger::mqttClient = PubSubClient(ethernetClient);
    Messenger::mqttClient.setCallback(Messenger::callback);
    Messenger::mqttClient.setBufferSize(1024);
}

Messenger *Messenger::get() {
    if (!messenger) {
        messenger = new Messenger();
    }
    return messenger;
}

bool Messenger::isConnected() {
    return mqttClient.connected();
}

bool Messenger::connect() {
    Serial.println("INFO:  Connecting to MQTT broker");
    if (!isConnected() && mqttClient.connect(MQTT_CLIENT_ID)) {
        Serial.println("INFO:  Connected");
        return true;
    }
    Serial.println("ERROR: Failed to connect");
    return false;
}

bool Messenger::setup() {
    Serial.println("INFO:  Initializing messenger");
    return isConnected() || connect();
}

bool Messenger::loop() {
    if (!isConnected() && !connect()) {
        Serial.println("ERROR: Failed to reconnect to MQTT broker");
        return false;
    }

    const ulong now = millis();
    if (now - lastHeartbeat > HEARTBEAT_INTERVAL) {
        publishHeartbeat();
        lastHeartbeat = now;
    }

    return mqttClient.loop();
}

bool Messenger::subscribe() {
    if (!isConnected() && !connect()) {
        return false;
    }
    return mqttClient.subscribe(TOPIC_CONTROL_DRIP_VALVE);
}

void Messenger::callback(char *topic, uint8_t *payload, uint length) {
    char m[length + 1];
    snprintf(m, length, "%s", payload);
    Serial.printf("INFO:  Received: %s => %s\n", topic, m);

    StaticJsonDocument<1024> message;
    deserializeJson(message, payload, length);

    Messenger::get()->handleMessage(String(topic), message);
}

bool Messenger::handleMessage(String topic, StaticJsonDocument<1024> message) {
    if (topic == TOPIC_CONTROL_DRIP_VALVE) {
        return EspNow::get()->sendDripValveControl(message["status"]);
    } else {
        Serial.printf("ERROR: Received message on unsupported topic: %s\n", topic.c_str());
        return false;
    }
}

bool Messenger::publish(String topic, StaticJsonDocument<1024> message) {
    if (!isConnected() && !connect()) {
        return false;
    }

    char json[1024];
    const size_t length = serializeJson(message, json);
    Serial.printf("INFO:  Sending: %s => %s\n", topic.c_str(), json);
    return mqttClient.publish(topic.c_str(), json, length);
}

bool Messenger::publishHeartbeat() {
    StaticJsonDocument<1024> message;
    message["timestamp"] = NTPTime::get()->now();
    return publish(String(TOPIC_HEARTBEAT), message);
}

bool Messenger::publishBatteryVoltage(Device *source, const float voltage) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["voltage"] = voltage;
    message["unit"] = "percent";
    return publish(String(TOPIC_SENSOR_BATTERY_VOLTAGE), message);
}

bool Messenger::publishWeatherTemperature(Device *source, const float temperature) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["temperature"] = temperature;
    message["unit"] = "celsius";
    return publish(String(TOPIC_SENSOR_WEATHER_TEMPERATURE), message);
}

bool Messenger::publishWeatherHumidity(Device *source, const float humidity) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["humidity"] = humidity;
    message["unit"] = "percent";
    return publish(String(TOPIC_SENSOR_WEATHER_HUMIDITY), message);
}

bool Messenger::publishWeatherPressure(Device *source, const float pressure) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["pressure"] = pressure;
    message["unit"] = "pascals";
    return publish(String(TOPIC_SENSOR_WEATHER_PRESSURE), message);
}

bool Messenger::publishWeatherLight(Device *source, const float light) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["light"] = light;
    message["unit"] = "percent";
    return publish(String(TOPIC_SENSOR_WEATHER_LIGHT), message);
}

bool Messenger::publishTankVolume(Device *source, const float volume) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["volume"] = volume;
    message["unit"] = "gallons";
    return publish(String(TOPIC_SENSOR_TANK_VOLUME), message);
}

bool Messenger::publishDripValveStatus(Device *source, const boolean status) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["timestamp"] = NTPTime::get()->now();
    message["status"] = status;
    return publish(String(TOPIC_STATUS_DRIP_VALVE), message);
}
