
#ifndef _REST_CLIENT_HPP
#define _REST_CLIENT_HPP


#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <stdint.h>
#include "Device.hpp"
#include "control/ControlDripValve.hpp"
#include "net/NetworkTime.hpp"


class RestClient
{
private:
    NetworkTime *networkTime;
    HTTPClient httpClient;

protected:
    bool setup();

    int doGet(String url, StaticJsonDocument<1024> message);
    int doPost(String url, StaticJsonDocument<1024> message);

public:
    RestClient(NetworkTime *networkTime);
    static bool get(RestClient **ref, NetworkTime *networkTime);

    bool publishBatteryVoltage(Device *device, const float voltage);
    bool publishWeatherTemperature(Device *device, const float temperature);
    bool publishWeatherHumidity(Device *device, const float humidity);
    bool publishWeatherPressure(Device *device, const float pressure);
    bool publishWeatherLight(Device *device, const float light);
    bool publishTankDistance(Device *device, const uint8_t tank, const float distance);
    bool publishDripValveState(Device *device, const DripValveState state);
};


#endif
