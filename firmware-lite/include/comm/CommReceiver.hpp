
#ifndef _GARDEN_AUTOMATION_COMM_RECEIVER_HPP
#define _GARDEN_AUTOMATION_COMM_RECEIVER_HPP


#include <esp_now.h>
#include <stdint.h>
#include "comm/Messages.hpp"
#include "util/Mac.hpp"


class CommReceiver
{
public:
    CommReceiver();

    bool setup();

protected:
    bool recv(Mac receiver, const uint8_t type, const uint8_t *payload, const uint8_t size);
};


#endif
