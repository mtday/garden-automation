
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "control/ControlDripValve.hpp"
#include "net/Messenger.hpp"
#include "net/NetworkTime.hpp"


static Messenger *messenger;


Messenger::Messenger(NetworkTime *networkTime) {
    Messenger::networkTime = networkTime;

    IPAddress brokerIP = IPAddress();
    brokerIP.fromString(MQTT_BROKER_IP);
    EthernetClient ethernetClient;

    Messenger::mqttClient = PubSubClient(ethernetClient);
    Messenger::mqttClient.setCallback(Messenger::callback);
    Messenger::mqttClient.setBufferSize(1024);
}

bool Messenger::get(Messenger **ref, NetworkTime *networkTime) {
    if (messenger) {
        *ref = messenger;
        return true;
    }
    messenger = new Messenger(networkTime);
    if (!messenger->setup()) {
        messenger = *ref = NULL;
        return false;
    }
    *ref = messenger;
    return true;
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
    Serial.println("ERROR: Failed to connect to MQTT broker");
    return false;
}

bool Messenger::setup() {
    Serial.println("INFO:  Initializing connection to MQTT broker");
    if (!isConnected() && !connect()) {
        Serial.println("ERROR: Failed to connect to MQTT broker");
        return false;
    }

    if (Device::get()->getType() == DeviceTypeTankGroup) {
        return subscribe(TOPIC_VALVE_DRIP_CONTROL);
    } else {
        // No need to subscribe to any topics
        return true;
    }
}

bool Messenger::loop() {
    if (!isConnected() && !connect()) {
        Serial.println("ERROR: Failed to reconnect to MQTT broker");
        return false;
    }

    if (!mqttClient.loop()) {
        Serial.println("ERROR: Failed to loop MQTT broker");
        return false;
    }
    return true;
}

bool Messenger::subscribe(String topic) {
    if (!isConnected() && !connect()) {
        return false;
    }
    return mqttClient.subscribe(topic.c_str());
}

void Messenger::callback(char *topic, uint8_t *payload, uint length) {
    char m[length + 1];
    snprintf(m, length, "%s", payload);
    Serial.printf("INFO:  Received: %s => %s\n", topic, m);

    StaticJsonDocument<1024> message;
    deserializeJson(message, payload, length);

    messenger->handleMessage(String(topic), message);
}

bool Messenger::handleMessage(String topic, StaticJsonDocument<1024> message) {
    if (topic == TOPIC_VALVE_DRIP_CONTROL) {
        ControlDripValve *dripValve;
        if (!ControlDripValve::get(&dripValve, Device::get()->getType())) {
            Serial.println("ERROR: Failed to create drip valve");
            return false;
        }
        const DripValveState state = message["state"];
        switch (state) {
            case DripValveStateOpen: {
                if (dripValve->getState() == DripValveStateOpen) {
                    // Already open
                    Serial.println("WARN:  Received drip valve open request, but already open");
                    return publishDripValveState(Device::get(), dripValve->getState());
                } else if (dripValve->open()) {
                    // Opened
                    Serial.println("INFO:  Drip valve successfully opened");
                    if (!publishDripValveState(Device::get(), dripValve->getState())) {
                        // Failed to notify that drip valve was opened, close it back to be safe
                        Serial.println("ERROR: Failed to notify of opened drip valve, closing the valve");
                        if (dripValve->close()) {
                            Serial.println("INFO:  Drip valve successfully closed");
                        } else {
                            Serial.println("WARN:  Failed to close drip valve");
                        }
                        return false;
                    } else {
                        return true;
                    }
                } else {
                    // Failed to open
                    Serial.println("ERROR: Received drip valve open request, and failed to open");
                    publishDripValveState(Device::get(), dripValve->getState());
                    return false;
                }
            }
    
            case DripValveStateClosed: {
                if (dripValve->getState() == DripValveStateClosed) {
                    // Already closed
                    Serial.println("WARN:  Received drip valve close request, but already closed");
                    return publishDripValveState(Device::get(), dripValve->getState());
                } else if (dripValve->close()) {
                    // Closed
                    Serial.println("INFO:  Drip valve successfully closed");
                    return publishDripValveState(Device::get(), dripValve->getState());
                } else {
                    // Failed to close
                    Serial.println("ERROR: Received drip valve close request, and failed to close");
                    publishDripValveState(Device::get(), dripValve->getState());
                    return false;
                }
            }
        }
    } else {
        Serial.printf("ERROR: Received message on unsupported topic: %s\n", topic.c_str());
        return false;
    }
    return true;
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

bool Messenger::publishBatteryVoltage(Device *source, const float voltage) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["voltage"] = voltage;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_BATTERY_VOLTAGE), message);
}

bool Messenger::publishWeatherTemperature(Device *source, const float temperature) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["temperature"] = temperature;
    message["unit"] = "celsius";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_WEATHER_TEMPERATURE), message);
}

bool Messenger::publishWeatherHumidity(Device *source, const float humidity) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["humidity"] = humidity;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_WEATHER_HUMIDITY), message);
}

bool Messenger::publishWeatherPressure(Device *source, const float pressure) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["pressure"] = pressure;
    message["unit"] = "pascals";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_WEATHER_PRESSURE), message);
}

bool Messenger::publishWeatherLight(Device *source, const float light) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["light"] = light;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_WEATHER_LIGHT), message);
}

bool Messenger::publishTankDistance(Device *source, const uint8_t tank, const float distance) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["tank"] = tank;
    message["distance"] = distance;
    message["unit"] = "centimeters";
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_SENSOR_TANK_DISTANCE), message);
}

bool Messenger::publishDripValveState(Device *source, const DripValveState state) {
    StaticJsonDocument<1024> message;
    message["source"] = source->getMac().c_str();
    message["state"] = state;
    message["timestamp"] = networkTime->isotime();
    return publish(String(TOPIC_VALVE_DRIP_STATE), message);
}
