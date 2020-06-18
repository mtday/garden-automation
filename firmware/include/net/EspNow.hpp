
#ifndef _ESP_NOW_HPP
#define _ESP_NOW_HPP


#include <esp_now.h>
#include <stdint.h>
#include "Device.hpp"
#include "util/Mac.hpp"


typedef struct {
    float voltage;
} BatteryData;


typedef struct {
    float temperature;
    float humidity;
    float pressure;
} WeatherData;


typedef struct {
    float light;
} LightData;


typedef struct {
    float distance;
} TankData;


typedef struct {
    bool status;
} DripValveControl;


typedef struct {
    bool status;
} DripValveData;


enum MessageType {
    MessageTypeBattery = 'B',
    MessageTypeWeather = 'W',
    MessageTypeLight = 'L',
    MessageTypeTank = 'D',
    MessageTypeDripValve = 'S',
    MessageTypeDripValveControl = 'V',
};


class EspNow {
public:
    EspNow();
    static EspNow *get();

    bool setup();

    bool sendBattery(const float voltage);
    bool sendWeather(const float temperature, const float humidity, const float pressure);
    bool sendLight(const float light);
    bool sendTank(const float distance);
    bool sendDripValve(const bool status);
    bool sendDripValveControl(const bool status);

    bool recvBattery(Device *source, const float voltage);
    bool recvWeather(Device *source, const float temperature, const float humidity, const float pressure);
    bool recvLight(Device *source, const float light);
    bool recvTank(Device *source, const float distance);
    bool recvDripValve(Device *source, const bool status);
    bool recvDripValveControl(Device *source, const bool status);

protected:
    static bool send(Mac receiver, const MessageType type, const uint8_t *payload, const uint8_t size);
    static void recv(const uint8_t *mac, const uint8_t *payload, const int size);
};


#endif
