
#ifndef _GARDEN_AUTOMATION_COMM_SENDER_HPP
#define _GARDEN_AUTOMATION_COMM_SENDER_HPP


#include <esp_now.h>
#include <stdint.h>
#include "comm/Messages.hpp"
#include "util/Mac.hpp"


class CommSender
{
public:
    CommSender();

    bool setup();

    bool sendWeather(const float temperature, const float humidity, const float pressure, const float light);
    bool sendTankVolume(const float volume);
    bool sendTankValveControl(const bool status);

protected:
    static bool send(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t size);
};


#endif
