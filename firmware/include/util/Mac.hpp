
#ifndef _GARDEN_AUTOMATION_UTIL_MAC_HPP
#define _GARDEN_AUTOMATION_UTIL_MAC_HPP


#include <stdint.h>
#include <WString.h>
#include <WiFi.h>


class Mac {
private:
    uint8_t bytes[6];
    String str;

public:
    Mac();
    Mac(const uint8_t *mac);
    Mac(uint8_t one, uint8_t two, uint8_t three, uint8_t four, uint8_t five, uint8_t six);

    bool operator==(const Mac& mac) const;
    bool operator==(const uint8_t *bytes) const;

    uint8_t operator[](const int index) const;

    const uint8_t *getAddress();
    const char *c_str();
};


#endif
