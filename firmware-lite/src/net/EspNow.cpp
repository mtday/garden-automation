
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

bool EspNow::sendWeather(const float temperature, const float humidity, const float pressure, const float light) {
    WeatherData weather;
    weather.temperature = temperature;
    weather.humidity = humidity;
    weather.pressure = pressure;
    weather.light = light;

    Serial.printf("INFO:  Sending weather data message: %f, %f, %f, %f\n",
            temperature, humidity, pressure, light);
    return send(MAC_CONTROLLER, MessageTypeWeather, (uint8_t *) &weather, sizeof(weather));
}

bool EspNow::sendTankVolume(const float volume) {
    TankVolumeData tankVolume;
    tankVolume.volume = volume;

    Serial.printf("INFO:  Sending tank volume data message: %f\n", volume);
    return send(MAC_CONTROLLER, MessageTypeTankVolume, (uint8_t *) &tankVolume, sizeof(tankVolume));
}

bool EspNow::sendDripValveControl(const bool status) {
    DripValveControl dripValveControl;
    dripValveControl.status = status;

    Serial.printf("INFO:  Sending tank valve control request message: %s\n", status ? "open" : "close");
    return send(MAC_DRIP_VALVE, MessageTypeDripValveControl, (uint8_t *) &dripValveControl, sizeof(dripValveControl));
}

bool EspNow::sendDripValveStatus(const bool status) {
    DripValveStatus dripValveStatus;
    dripValveStatus.status = status;

    Serial.printf("INFO:  Sending tank valve status message: %s\n", status ? "opened" : "closed");
    return send(MAC_CONTROLLER, MessageTypeDripValveStatus, (uint8_t *) &dripValveStatus, sizeof(dripValveStatus));
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
        case MessageTypeWeather:
            if (sizeof(WeatherData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW weather data message size: %d\n", size);
                return;
            }
            WeatherData weather;
            memcpy(&weather, payload + 1, size - 1);
            EspNow::get()->recvWeather(
                    source, weather.temperature, weather.humidity, weather.pressure, weather.light);
            break;
        case MessageTypeTankVolume: // tank volume
            if (sizeof(TankVolumeData) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW tank volume data message size: %d\n", size);
                return;
            }
            TankVolumeData tankVolume;
            memcpy(&tankVolume, payload + 1, size - 1);
            EspNow::get()->recvTankVolume(source, tankVolume.volume);
            break;
        case MessageTypeDripValveControl: // drip valve control
            if (sizeof(DripValveControl) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve control message size: %d\n", size);
                return;
            }
            DripValveControl dripValveControl;
            memcpy(&dripValveControl, payload + 1, size - 1);
            EspNow::get()->recvDripValveControl(source, dripValveControl.status);
            break;
        case MessageTypeDripValveStatus: // drip valve status
            if (sizeof(DripValveStatus) + 1 != size) {
                Serial.printf("ERROR: Unexpected ESP-NOW drip valve status message size: %d\n", size);
                return;
            }
            DripValveStatus dripValveStatus;
            memcpy(&dripValveStatus, payload + 1, size - 1);
            EspNow::get()->recvDripValveStatus(source, dripValveStatus.status);
            break;
        default:
            Serial.printf("ERROR: Unrecognized message type: %c", type);
            return;
    }
}

bool EspNow::recvWeather(
    Device *source,
    const float temperature,
    const float humidity,
    const float pressure,
    const float light
) {
    Serial.printf("INFO:  Received weather from %s: %f, %f, %f, %f\n",
            source->c_str(), temperature, humidity, pressure, light);
    return Messenger::get()->publishWeatherTemperature(source, temperature)
            && Messenger::get()->publishWeatherHumidity(source, humidity)
            && Messenger::get()->publishWeatherPressure(source, pressure)
            && Messenger::get()->publishWeatherLight(source, light);
}

bool EspNow::recvTankVolume(Device *source, const float volume) {
    Serial.printf("INFO:  Received tank volume from %s: %f\n", source->c_str(), volume);
    return Messenger::get()->publishTankVolume(source, volume);
}

bool EspNow::recvDripValveStatus(Device *source, const bool status)
{
    Serial.printf("INFO:  Received drip valve status from %s: %s\n", source->c_str(), status ? "opened" : "closed");
    return Messenger::get()->publishDripValveStatus(source, status);
}

bool EspNow::recvDripValveControl(Device *source, const bool status) {
    Serial.printf("INFO:  Received drip valve control from %s: %s\n", source->c_str(), status ? "open" : "close");
    // TODO: control the valve
    return EspNow::get()->sendDripValveStatus(status);
}
