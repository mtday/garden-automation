
#include "comm/CommSender.hpp"


CommSender::CommSender()
{
}

bool CommSender::setup()
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ERROR: Failed to initialize ESP-NOW");
        return false;
    }
    return true;
}

bool CommSender::sendWeather(const float temperature, const float humidity, const float pressure, const float light)
{
    WeatherData weather;
    weather.temperature = temperature;
    weather.humidity = humidity;
    weather.pressure = pressure;
    weather.light = light;

    return send(DEVICE_AUTOMATION_CONTROL, 'W', (uint8_t *) &weather, sizeof(weather));
}

bool CommSender::sendTankVolume(const float volume)
{
    TankVolumeData tankVolume;
    tankVolume.volume = volume;

    return send(DEVICE_AUTOMATION_CONTROL, 'V', (uint8_t *) &tankVolume, sizeof(tankVolume));
}

bool CommSender::sendTankValveControl(const bool status)
{
    TankValveControl tankValveControl;
    tankValveControl.status = status;

    return send(DEVICE_AUTOMATION_CONTROL, 'O', (uint8_t *) &tankValveControl, sizeof(tankValveControl));
}

bool CommSender::send(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t length)
{
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
