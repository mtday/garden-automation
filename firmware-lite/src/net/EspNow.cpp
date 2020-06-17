
#include "Device.hpp"
#include "net/EspNow.hpp"


static EspNow *espNow;


EspNow::EspNow()
{
}

EspNow *EspNow::get() {
    if (!espNow) {
        espNow = new EspNow();
    }
    return espNow;
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

    Serial.printf("INFO:  Sending weather data message: %f, %f, %f, %f\n",
            temperature, humidity, pressure, light);
    return send(DEVICE_AUTOMATION_CONTROL, 'W', (uint8_t *) &weather, sizeof(weather));
}

bool EspNow::sendTankVolume(const float volume)
{
    TankVolumeData tankVolume;
    tankVolume.volume = volume;

    Serial.printf("INFO:  Sending tank volume data message: %f\n", volume);
    return send(DEVICE_AUTOMATION_CONTROL, 'V', (uint8_t *) &tankVolume, sizeof(tankVolume));
}

bool EspNow::sendTankValveControl(const bool status)
{
    TankValveControl tankValveControl;
    tankValveControl.status = status;

    Serial.printf("INFO:  Sending tank valve control request message: %s\n", status ? "open" : "close");
    return send(DEVICE_TANK_VALVE, 'C', (uint8_t *) &tankValveControl, sizeof(tankValveControl));
}

bool EspNow::sendTankValveStatus(const bool status)
{
    TankValveControl tankValveControl;
    tankValveControl.status = status;

    Serial.printf("INFO:  Sending tank valve status message: %s\n", status ? "opened" : "closed");
    return send(DEVICE_AUTOMATION_CONTROL, 'S', (uint8_t *) &tankValveControl, sizeof(tankValveControl));
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

void EspNow::recv(const uint8_t *mac, const uint8_t *payload, const int size)
{
    if (size == 0) {
        Serial.println("ERROR: Unexpected empty ESP-NOW message");
        return;
    }

    Mac source(mac);
    const uint8_t type = payload[0];

    switch (type) {
        case 'W': // weather
            if (sizeof(WeatherData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW weather data message size: %d\n", size);
                return;
            }
            WeatherData weather;
            memcpy(&weather, payload + 1, size - 1);
            EspNow::get()->recvWeather(
                    source, weather.temperature, weather.humidity, weather.pressure, weather.light);
            break;
        case 'V': // tank volume
            if (sizeof(TankVolumeData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank volume data message size: %d\n", size);
                return;
            }
            TankVolumeData tankVolume;
            memcpy(&tankVolume, payload + 1, size - 1);
            EspNow::get()->recvTankVolume(source, tankVolume.volume);
            break;
        case 'C': // valve control
            if (sizeof(TankValveControl) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank valve control message size: %d\n", size);
                return;
            }
            TankValveControl tankValveControl;
            memcpy(&tankValveControl, payload + 1, size - 1);
            EspNow::get()->recvTankValveControl(source, tankValveControl.status);
            break;
        case 'S': // valve status
            if (sizeof(TankValveStatus) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank valve status message size: %d\n", size);
                return;
            }
            TankValveStatus tankValveStatus;
            memcpy(&tankValveStatus, payload + 1, size - 1);
            EspNow::get()->recvTankValveStatus(source, tankValveStatus.status);
            break;
        default:
            Serial.printf("ERROR: Unrecognized message type: %c", type);
            return;
    }
}

bool EspNow::recvWeather(
    Mac source,
    const float temperature,
    const float humidity,
    const float pressure,
    const float light
) {
    Serial.printf("INFO:  Received weather from %s: %f, %f, %f, %f\n",
            source.c_str(), temperature, humidity, pressure, light);
    if (Device::get()->getType() == Controller) {
        return Messenger::get()->publishWeatherTemperature(source, temperature)
                && Messenger::get()->publishWeatherHumidity(source, humidity)
                && Messenger::get()->publishWeatherPressure(source, pressure)
                && Messenger::get()->publishWeatherLight(source, light);
    } else {
        Serial.println("ERROR: Unexpectedly received weather data message");
        return false;
    }
}

bool EspNow::recvTankVolume(Mac source, const float volume)
{
    Serial.printf("INFO:  Received tank volume from %s: %f\n", source.c_str(), volume);
    if (Device::get()->getType() == Controller) {
        return Messenger::get()->publishTankVolume(source, volume);
    } else {
        Serial.println("ERROR: Unexpectedly received tank volume data message");
        return false;
    }
}

bool EspNow::recvTankValveControl(Mac source, const bool status)
{
    Serial.printf("INFO:  Received tank valve control from %s: %s\n", source.c_str(), status ? "open" : "close");
    if (Device::get()->getType() == TankValve) {
        // TODO: control the valve
        return EspNow::get()->sendTankValveStatus(status);
    } else {
        Serial.println("ERROR: Unexpectedly received tank valve control message");
        return false;
    }
}

bool EspNow::recvTankValveStatus(Mac source, const bool status)
{
    Serial.printf("INFO:  Received tank valve status from %s: %s\n", source.c_str(), status ? "opened" : "closed");
    if (Device::get()->getType() == Controller) {
        return Messenger::get()->publishTankValveStatus(source, status);
    } else {
        Serial.println("ERROR: Unexpectedly received tank valve control message");
        return false;
    }
}
