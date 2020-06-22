
#include <Arduino.h>
#include <WiFi.h>
#include "net/EspNow.hpp"


static EspNow *espNow;

EspNow::EspNow()
{
}

bool EspNow::get(EspNow **ref)
{
    if (espNow)
    {
        *ref = espNow;
        return true;
    }
    espNow = new EspNow();
    if (!espNow->setup())
    {
        espNow = *ref = NULL;
        return false;
    }
    *ref = espNow;
    return true;
}

bool EspNow::setup()
{
    Serial.println("INFO:  Initializing ESP-NOW");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.channel(ESPNOW_CHANNEL);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ERROR: Failed to initialize ESP-NOW");
        return false;
    }

    if (esp_now_register_recv_cb(EspNow::recv) != ESP_OK)
    {
        Serial.println("ERROR: Failed to add ESP-NOW receive callback");
        return false;
    }

    return true;
}

bool EspNow::sendBattery(const float voltage)
{
    BatteryData batteryData;
    batteryData.voltage = voltage;

    Serial.printf("INFO:  Sending battery data message: %f\n", voltage);
    return send(MAC_BROADCAST, MessageTypeBattery, (uint8_t *) &batteryData, sizeof(batteryData));
}

bool EspNow::sendWeather(const float temperature, const float humidity, const float pressure)
{
    WeatherData weatherData;
    weatherData.temperature = temperature;
    weatherData.humidity = humidity;
    weatherData.pressure = pressure;

    Serial.printf("INFO:  Sending weather data message: %f, %f, %f\n", temperature, humidity, pressure);
    return send(MAC_BROADCAST, MessageTypeWeather, (uint8_t *) &weatherData, sizeof(weatherData));
}

bool EspNow::sendLight(const float light)
{
    LightData lightData;
    lightData.light = light;

    Serial.printf("INFO:  Sending light data message: %f\n", light);
    return send(MAC_BROADCAST, MessageTypeBattery, (uint8_t *) &lightData, sizeof(lightData));
}

bool EspNow::sendTankDistance(const uint8_t tank, const float distance)
{
    TankData tankData;
    tankData.tank = tank;
    tankData.distance = distance;

    Serial.printf("INFO:  Sending tank data message: %f\n", distance);
    return send(MAC_BROADCAST, MessageTypeTank, (uint8_t *) &tankData, sizeof(tankData));
}

bool EspNow::sendDripValve(const DripValveState state)
{
    DripValveData dripValveData;
    dripValveData.state = state;

    Serial.printf("INFO:  Sending drip valve data message: %s\n", state ? "opened" : "closed");
    return send(MAC_BROADCAST, MessageTypeDripValve, (uint8_t *) &dripValveData, sizeof(dripValveData));
}

bool EspNow::sendDripValveControl(const DripValveState state)
{
    DripValveControlData dripValveControlData;
    dripValveControlData.state = state;

    Serial.printf("INFO:  Sending drip valve control message: %s\n", state ? "open" : "close");
    return send(MAC_BROADCAST, MessageTypeDripValveControl,
            (uint8_t *) &dripValveControlData, sizeof(dripValveControlData));
}

bool EspNow::send(Mac receiver, const MessageType type, const uint8_t *payload, const uint8_t length)
{
    if (length + 1 > 250)
    {
        Serial.printf("ERROR: Message is too big for ESP-NOW: %d\n", length + 1);
        return false;
    }

    esp_now_peer_info_t peer;
    memcpy(peer.peer_addr, receiver.getAddress(), 6);
    peer.channel = ESPNOW_CHANNEL;
    peer.encrypt = false;

    if (esp_now_add_peer(&peer) != ESP_OK)
    {
        Serial.println("ERROR: Failed to add ESP-NOW peer");
        return false;
    }

    uint8_t data[length + 1];
    data[0] = type;
    memcpy(&data[1], payload, length);

    if (esp_now_send(receiver.getAddress(), data, length + 1) != ESP_OK)
    {
        Serial.println("ERROR: Failed to send data over ESP-NOW");
        return false;
    }
    return true;
}

void EspNow::recv(const uint8_t *mac, const uint8_t *payload, const int size)
{
    if (size == 0)
    {
        Serial.println("ERROR: Unexpected empty ESP-NOW message");
        return;
    }

    Device *source = Device::get(Mac(mac));
    MessageType type = (MessageType) payload[0];

    switch (type)
    {
        case MessageTypeBattery:
        {
            if (sizeof(BatteryData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW battery data message size: %d\n", size);
                return;
            }
            BatteryData batteryData;
            memcpy(&batteryData, payload + 1, size - 1);
            espNow->recvBattery(source, batteryData.voltage);
            break;
        }

        case MessageTypeWeather:
        {
            if (sizeof(WeatherData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW weather data message size: %d\n", size);
                return;
            }
            WeatherData weatherData;
            memcpy(&weatherData, payload + 1, size - 1);
            espNow->recvWeather(source, weatherData.temperature, weatherData.humidity, weatherData.pressure);
            break;
        }

        case MessageTypeLight:
        {
            if (sizeof(LightData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW light data message size: %d\n", size);
                return;
            }
            LightData lightData;
            memcpy(&lightData, payload + 1, size - 1);
            espNow->recvLight(source, lightData.light);
            break;
        }

        case MessageTypeTank:
        {
            if (sizeof(TankData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW tank data message size: %d\n", size);
                return;
            }
            TankData tankData;
            memcpy(&tankData, payload + 1, size - 1);
            espNow->recvTankDistance(source, tankData.tank, tankData.distance);
            break;
        }

        case MessageTypeDripValve:
        {
            if (sizeof(DripValveData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve data message size: %d\n", size);
                return;
            }
            DripValveData dripValveData;
            memcpy(&dripValveData, payload + 1, size - 1);
            espNow->recvDripValve(source, dripValveData.state);
            break;
        }

        case MessageTypeDripValveControl:
        {
            if (sizeof(DripValveControlData) + 1 != size)
            {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve control message size: %d\n", size);
                return;
            }
            DripValveControlData dripValveControlData;
            memcpy(&dripValveControlData, payload + 1, size - 1);
            espNow->recvDripValveControl(source, dripValveControlData.state);
            break;
        }

        default:
            Serial.printf("ERROR: Unrecognized message type: %c", type);
            return;
    }
}

bool EspNow::recvBattery(Device *source, const float voltage)
{
    Serial.printf("INFO:  Received battery data from %s: %f\n", source->c_str(), voltage);
    return true;
}

bool EspNow::recvWeather(Device *source, const float temperature, const float humidity, const float pressure)
{
    Serial.printf("INFO:  Received weather data from %s: %f, %f, %f\n",
            source->c_str(), temperature, humidity, pressure);
    return true;
}

bool EspNow::recvLight(Device *source, const float light)
{
    Serial.printf("INFO:  Received light data from %s: %f\n", source->c_str(), light);
    return true;
}

bool EspNow::recvTankDistance(Device *source, const uint8_t tank, const float distance)
{
    Serial.printf("INFO:  Received tank %d data from %s: %f\n", tank, source->c_str(), distance);
    return true;
}

bool EspNow::recvDripValve(Device *source, const DripValveState state)
{
    Serial.printf("INFO:  Received drip valve data from %s: %s\n", source->c_str(), state ? "opened" : "closed");
    return true;
}

bool EspNow::recvDripValveControl(Device *source, const DripValveState state)
{
    Serial.printf("INFO:  Received drip valve control from %s: %s\n", source->c_str(), state ? "open" : "close");
    return true;
}