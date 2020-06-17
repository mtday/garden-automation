
#ifndef _GARDEN_AUTOMATION_NET_ESPNOW_HPP
#define _GARDEN_AUTOMATION_NET_ESPNOW_HPP


#include <esp_now.h>
#include <stdint.h>
#include "util/Mac.hpp"


typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float light;
} WeatherData;


typedef struct
{
    float volume;
} TankVolumeData;


typedef struct
{
    bool status;
} TankValveControl;


typedef struct
{
    bool status;
} TankValveStatus;


class EspNow
{
public:
    EspNow();
    static EspNow *get();

    bool setup();

    bool sendWeather(const float temperature, const float humidity, const float pressure, const float light);
    bool sendTankVolume(const float volume);
    bool sendTankValveStatus(const bool status);
    bool sendTankValveControl(const bool status);

    bool recvWeather(Mac source, const float temperature, const float humidity, const float pressure, const float light);
    bool recvTankVolume(Mac source, const float volume);
    bool recvTankValveStatus(Mac source, const bool status);
    bool recvTankValveControl(Mac source, const bool status);

protected:
    static bool send(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t size);
    static void recv(const uint8_t *mac, const uint8_t *payload, const int size);
};


#endif
