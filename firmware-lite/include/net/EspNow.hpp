
#ifndef _GARDEN_AUTOMATION_NET_ESPNOW_HPP
#define _GARDEN_AUTOMATION_NET_ESPNOW_HPP


#include <esp_now.h>
#include <stdint.h>
#include "Device.hpp"
#include "util/Mac.hpp"


typedef struct {
    float temperature;
    float humidity;
    float pressure;
    float light;
} WeatherData;


typedef struct {
    float volume;
} TankVolumeData;


typedef struct {
    bool status;
} DripValveControl;


typedef struct {
    bool status;
} DripValveStatus;

enum MessageType {
    MessageTypeWeather = 'W',
    MessageTypeTankVolume = 'V',
    MessageTypeDripValveControl = 'D',
    MessageTypeDripValveStatus = 'S',
};


class EspNow {
public:
    EspNow();
    static EspNow *get();

    bool setup();

    bool sendWeather(const float temperature, const float humidity, const float pressure, const float light);
    bool sendTankVolume(const float volume);
    bool sendDripValveStatus(const bool status);
    bool sendDripValveControl(const bool status);

    bool recvWeather(Device *source, const float temperature, const float humidity, const float pressure, const float light);
    bool recvTankVolume(Device *source, const float volume);
    bool recvDripValveStatus(Device *source, const bool status);
    bool recvDripValveControl(Device *source, const bool status);

protected:
    static bool send(Mac receiver, const MessageType type, const uint8_t *payload, const uint8_t size);
    static void recv(const uint8_t *mac, const uint8_t *payload, const int size);
};


#endif
