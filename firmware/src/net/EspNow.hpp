
#ifndef _ESP_NOW_HPP
#define _ESP_NOW_HPP


#include <esp_now.h>
#include <stdint.h>
#include "Device.hpp"
#include "control/ControlDripValve.hpp"
#include "util/Mac.hpp"


typedef struct
{
    float voltage;
} BatteryData;


typedef struct
{
    float temperature;
    float humidity;
    float pressure;
} WeatherData;


typedef struct
{
    float light;
} LightData;


typedef struct
{
    uint8_t tank;
    float distance;
} TankData;


typedef struct
{
    DripValveState state;
} DripValveControlData;


typedef struct
{
    DripValveState state;
} DripValveData;


enum MessageType
{
    MessageTypeBattery = 'B',
    MessageTypeWeather = 'W',
    MessageTypeLight = 'L',
    MessageTypeTank = 'D',
    MessageTypeDripValve = 'S',
    MessageTypeDripValveControl = 'V',
};


class EspNow
{
protected:
    bool setup();

public:
    EspNow();
    static bool get(EspNow **ref);

    bool sendBattery(const float voltage);
    bool sendWeather(const float temperature, const float humidity, const float pressure);
    bool sendLight(const float light);
    bool sendTankDistance(const uint8_t tank, const float distance);
    bool sendDripValve(const DripValveState state);
    bool sendDripValveControl(const DripValveState state);

    bool recvBattery(Device *source, const float voltage);
    bool recvWeather(Device *source, const float temperature, const float humidity, const float pressure);
    bool recvLight(Device *source, const float light);
    bool recvTankDistance(Device *source, const uint8_t tank, const float distance);
    bool recvDripValve(Device *source, const DripValveState state);
    bool recvDripValveControl(Device *source, const DripValveState state);

protected:
    static bool send(Mac receiver, const MessageType type, const uint8_t *payload, const uint8_t size);
    static void recv(const uint8_t *mac, const uint8_t *payload, const int size);
};


#endif