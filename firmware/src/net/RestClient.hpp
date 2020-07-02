
#ifndef _REST_CLIENT_HPP
#define _REST_CLIENT_HPP


#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <stdint.h>
#include "control/ControlDripValve.hpp"
#include "net/NetworkTime.hpp"


class RestClient
{
private:
    HTTPClient httpClient;

protected:
    bool setup();

    int doGet(String url, StaticJsonDocument<1024> message);
    int doPost(String url, StaticJsonDocument<1024> message);

public:
    RestClient();
    static bool get(RestClient **ref);

    bool publishBatteryVoltage(const float voltage);
    bool publishWeatherTemperature(const float temperature);
    bool publishWeatherHumidity(const float humidity);
    bool publishWeatherPressure(const float pressure);
    bool publishWeatherLight(const float light);
    bool publishTankDistance(const float distance);
    bool publishDripValveState(const DripValveState state);
};


#endif
