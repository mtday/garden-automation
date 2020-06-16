
#ifndef _GARDEN_AUTOMATION_COMM_ESPNOW_HPP
#define _GARDEN_AUTOMATION_COMM_ESPNOW_HPP


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


class EspNow
{
public:
    EspNow();

    bool setup();

    bool sendWeather(const float temperature, const float humidity, const float pressure, const float light);
    bool sendTankVolume(const float volume);
    bool sendTankValveControl(const bool status);

protected:
    static void recv(const uint8_t *mac, const uint8_t *payload, const int size);
    static bool send(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t size);
};


#endif
