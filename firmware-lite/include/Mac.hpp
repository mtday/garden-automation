
#ifndef _GARDEN_AUTOMATION_MAC_H
#define _GARDEN_AUTOMATION_MAC_H


#include <stdint.h>
#include <WString.h>
#include <WiFi.h>

using namespace std;


class Mac
{
private:
    uint8_t bytes[6];
    String str;

public:
    Mac();
    Mac(uint8_t one, uint8_t two, uint8_t three, uint8_t four, uint8_t five, uint8_t six);

    bool operator==(const Mac& mac) const;
    bool operator==(const uint8_t *bytes) const;

    uint8_t operator[](const int index) const;

    const char *c_str();
};


#endif
