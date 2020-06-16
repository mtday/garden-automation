
#include "Device.hpp"
#include "comm/EspNow.hpp"


EspNow::EspNow()
{
}

bool EspNow::setup()
{
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

bool EspNow::sendWeather(const float temperature, const float humidity, const float pressure, const float light)
{
    WeatherData weather;
    weather.temperature = temperature;
    weather.humidity = humidity;
    weather.pressure = pressure;
    weather.light = light;

    Serial.println("INFO:  Sending weather data message");
    return send(DEVICE_AUTOMATION_CONTROL, 'W', (uint8_t *) &weather, sizeof(weather));
}

bool EspNow::sendTankVolume(const float volume)
{
    TankVolumeData tankVolume;
    tankVolume.volume = volume;

    Serial.println("INFO:  Sending tank volume data message");
    return send(DEVICE_AUTOMATION_CONTROL, 'V', (uint8_t *) &tankVolume, sizeof(tankVolume));
}

bool EspNow::sendTankValveControl(const bool status)
{
    TankValveControl tankValveControl;
    tankValveControl.status = status;

    Serial.println("INFO:  Sending tank valve control message");
    return send(DEVICE_AUTOMATION_CONTROL, 'O', (uint8_t *) &tankValveControl, sizeof(tankValveControl));
}

void EspNow::recv(const uint8_t *mac, const uint8_t *payload, const int size)
{
    if (size == 0) {
        Serial.println("ERROR: Unexpected empty ESP-NOW message");
        return;
    }

    Mac source(mac);
    const uint8_t type = payload[0];

    Device *device = Device::getDevice();

    switch (type) {
        case 'W': // weather
            Serial.println("INFO:  Received weather data message");
            if (sizeof(WeatherData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW weather data message size: %d\n", size);
                return;
            }
            WeatherData weather;
            memcpy(&weather, payload + 1, size - 1);
            device->weather(source, weather.temperature, weather.humidity, weather.pressure, weather.light);
            break;
        case 'V': // tank volume
            Serial.println("INFO:  Received tank volume data message");
            if (sizeof(TankVolumeData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank volume data message size: %d\n", size);
                return;
            }
            TankVolumeData tankVolume;
            memcpy(&tankVolume, payload + 1, size - 1);
            device->tankVolume(source, tankVolume.volume);
            break;
        case 'O': // valve control
            Serial.println("INFO:  Received tank valve control message");
            if (sizeof(TankValveControl) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank valve control message size: %d\n", size);
                return;
            }
            TankValveControl tankValve;
            memcpy(&tankValve, payload + 1, size - 1);
            device->tankValve(source, tankValve.status);
            break;
        default:
            Serial.printf("ERROR: Unrecognized message type: %c", type);
            return;
    }
}

bool EspNow::send(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t length)
{
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
