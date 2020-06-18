
#include <Arduino.h>
#include <WiFi.h>
#include "Runner.hpp"
#include "net/EspNow.hpp"


static EspNow *espNow;

EspNow::EspNow() {
}

EspNow *EspNow::get() {
    if (!espNow) {
        espNow = new EspNow();
    }
    return espNow;
}

bool EspNow::setup() {
    Serial.println("INFO:  Initializing ESP-NOW");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.channel(ESPNOW_CHANNEL);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ERROR: Failed to initialize ESP-NOW");
        return false;
    }

    if (esp_now_register_recv_cb(EspNow::recv) != ESP_OK) {
        Serial.println("ERROR: Failed to add ESP-NOW receive callback");
        return false;
    }

    return true;
}

bool EspNow::sendBattery(const float voltage) {
    BatteryData batteryData;
    batteryData.voltage = voltage;

    Serial.printf("INFO:  Sending battery data message: %f\n", voltage);
    return send(MAC_CONTROLLER, MessageTypeBattery, (uint8_t *) &batteryData, sizeof(batteryData));
}

bool EspNow::sendWeather(const float temperature, const float humidity, const float pressure) {
    WeatherData weatherData;
    weatherData.temperature = temperature;
    weatherData.humidity = humidity;
    weatherData.pressure = pressure;

    Serial.printf("INFO:  Sending weather data message: %f, %f, %f\n", temperature, humidity, pressure);
    return send(MAC_CONTROLLER, MessageTypeWeather, (uint8_t *) &weatherData, sizeof(weatherData));
}

bool EspNow::sendLight(const float light) {
    LightData lightData;
    lightData.light = light;

    Serial.printf("INFO:  Sending light data message: %f\n", light);
    return send(MAC_CONTROLLER, MessageTypeBattery, (uint8_t *) &lightData, sizeof(lightData));
}

bool EspNow::sendTank(const float distance) {
    TankData tankData;
    tankData.distance = distance;

    Serial.printf("INFO:  Sending tank data message: %f\n", distance);
    return send(MAC_CONTROLLER, MessageTypeTank, (uint8_t *) &tankData, sizeof(tankData));
}

bool EspNow::sendDripValve(const bool status) {
    DripValveData dripValveData;
    dripValveData.status = status;

    Serial.printf("INFO:  Sending drip valve data message: %s\n", status ? "opened" : "closed");
    return send(MAC_CONTROLLER, MessageTypeDripValve, (uint8_t *) &dripValveData, sizeof(dripValveData));
}

bool EspNow::sendDripValveControl(const bool status) {
    DripValveControl dripValveControl;
    dripValveControl.status = status;

    Serial.printf("INFO:  Sending drip valve control message: %s\n", status ? "open" : "close");
    return send(MAC_DRIP_VALVE, MessageTypeDripValveControl, (uint8_t *) &dripValveControl, sizeof(dripValveControl));
}

bool EspNow::send(Mac receiver, const MessageType type, const uint8_t *payload, const uint8_t length) {
    if (length + 1 > 250) {
        Serial.printf("ERROR: Message is too big for ESP-NOW: %d\n", length + 1);
        return false;
    }

    esp_now_peer_info_t peer;
    memcpy(peer.peer_addr, receiver.getAddress(), 6);
    peer.channel = ESPNOW_CHANNEL;
    peer.encrypt = false;

    if (esp_now_add_peer(&peer) != ESP_OK) {
        Serial.println("ERROR: Failed to add ESP-NOW peer");
        return false;
    }

    uint8_t data[length + 1];
    data[0] = type;
    memcpy(&data[1], payload, length);

    if (esp_now_send(receiver.getAddress(), data, length + 1) != ESP_OK) {
        Serial.println("ERROR: Failed to send data over ESP-NOW");
        return false;
    }
    return true;
}

void EspNow::recv(const uint8_t *mac, const uint8_t *payload, const int size) {
    if (size == 0) {
        Serial.println("ERROR: Unexpected empty ESP-NOW message");
        return;
    }

    Device *source = Device::get(Mac(mac));
    MessageType type = (MessageType) payload[0];

    switch (type) {
        case MessageTypeBattery: {
            if (sizeof(BatteryData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW battery data message size: %d\n", size);
                return;
            }
            BatteryData batteryData;
            memcpy(&batteryData, payload + 1, size - 1);
            EspNow::get()->recvBattery(source, batteryData.voltage);
            break;
        }

        case MessageTypeWeather: {
            if (sizeof(WeatherData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW weather data message size: %d\n", size);
                return;
            }
            WeatherData weatherData;
            memcpy(&weatherData, payload + 1, size - 1);
            EspNow::get()->recvWeather(source, weatherData.temperature, weatherData.humidity, weatherData.pressure);
            break;
        }

        case MessageTypeLight: {
            if (sizeof(LightData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW light data message size: %d\n", size);
                return;
            }
            LightData lightData;
            memcpy(&lightData, payload + 1, size - 1);
            EspNow::get()->recvLight(source, lightData.light);
            break;
        }

        case MessageTypeTank: {
            if (sizeof(TankData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank data message size: %d\n", size);
                return;
            }
            TankData tankData;
            memcpy(&tankData, payload + 1, size - 1);
            EspNow::get()->recvTank(source, tankData.distance);
            break;
        }

        case MessageTypeDripValve: {
            if (sizeof(DripValveData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve data message size: %d\n", size);
                return;
            }
            DripValveData dripValveData;
            memcpy(&dripValveData, payload + 1, size - 1);
            EspNow::get()->recvDripValve(source, dripValveData.status);
            break;
        }

        case MessageTypeDripValveControl: {
            if (sizeof(DripValveControl) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve control message size: %d\n", size);
                return;
            }
            DripValveControl dripValveControl;
            memcpy(&dripValveControl, payload + 1, size - 1);
            EspNow::get()->recvDripValveControl(source, dripValveControl.status);
            break;
        }

        default:
            Serial.printf("ERROR: Unrecognized message type: %c", type);
            return;
    }
}

bool EspNow::recvBattery(Device *source, const float voltage) {
    Serial.printf("INFO:  Received battery data from %s: %f\n", source->c_str(), voltage);
    return Messenger::get()->publishBatteryVoltage(source, voltage);
}

bool EspNow::recvWeather(Device *source, const float temperature, const float humidity, const float pressure) {
    Serial.printf("INFO:  Received weather data from %s: %f, %f, %f\n",
            source->c_str(), temperature, humidity, pressure);
    return Messenger::get()->publishWeatherTemperature(source, temperature)
            && Messenger::get()->publishWeatherHumidity(source, humidity)
            && Messenger::get()->publishWeatherPressure(source, pressure);
}

bool EspNow::recvLight(Device *source, const float light) {
    Serial.printf("INFO:  Received light data from %s: %f\n", source->c_str(), light);
    return Messenger::get()->publishWeatherLight(source, light);
}

bool EspNow::recvTank(Device *source, const float distance) {
    Serial.printf("INFO:  Received tank data from %s: %f\n", source->c_str(), distance);
    return Messenger::get()->publishTankDistance(source, distance);
}

bool EspNow::recvDripValve(Device *source, const bool status)
{
    Serial.printf("INFO:  Received drip valve data from %s: %s\n", source->c_str(), status ? "opened" : "closed");
    return Messenger::get()->publishDripValveStatus(source, status);
}

bool EspNow::recvDripValveControl(Device *source, const bool status) {
    Serial.printf("INFO:  Received drip valve control from %s: %s\n", source->c_str(), status ? "open" : "close");
    // TODO: control the valve
    return EspNow::get()->sendDripValve(status);
}
