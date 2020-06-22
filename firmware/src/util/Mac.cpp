
#include <WiFi.h>
#include "util/Mac.hpp"


Mac::Mac()
{
    WiFi.macAddress(bytes);
    char macstr[18];
    snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
    str = String(macstr);
}

Mac::Mac(const uint8_t *mac)
{
    memcpy(bytes, mac, 6);
    char macstr[18];
    snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
    str = String(macstr);
}

Mac::Mac(uint8_t one, uint8_t two, uint8_t three, uint8_t four, uint8_t five, uint8_t six)
{
    bytes[0] = one;
    bytes[1] = two;
    bytes[2] = three;
    bytes[3] = four;
    bytes[4] = five;
    bytes[5] = six;

    char macstr[18];
    snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
    str = String(macstr);
}

bool Mac::operator==(const Mac& mac) const
{
    return str == mac.str;
}

bool Mac::operator==(const uint8_t *bytes) const
{
    return Mac::bytes[0] == bytes[0]
            && Mac::bytes[1] == bytes[1]
            && Mac::bytes[2] == bytes[2]
            && Mac::bytes[3] == bytes[3]
            && Mac::bytes[4] == bytes[4]
            && Mac::bytes[5] == bytes[5];
}

uint8_t Mac::operator[](const int index) const
{
    return bytes[index];
}

const uint8_t *Mac::getAddress()
{
    return bytes;
}

const char *Mac::c_str()
{
    return str.c_str();
}

