
#include "comm/CommReceiver.hpp"


CommReceiver::CommReceiver()
{
}

bool CommReceiver::setup()
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ERROR: Failed to initialize ESP-NOW");
        return false;
    }
    return true;
}

bool CommReceiver::recv(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t size)
{
    return true;
}
