
#include "net/RestClient.hpp"


static RestClient *restClient;
static const String baseUrl = String("http://") + NODE_RED_HOST + ":" + NODE_RED_PORT;


RestClient::RestClient(NetworkTime *networkTime)
{
    RestClient::networkTime = networkTime;
}

bool RestClient::get(RestClient **ref, NetworkTime *networkTime)
{
    if (restClient)
    {
        *ref = restClient;
        return true;
    }
    restClient = new RestClient(networkTime);
    if (!restClient->setup())
    {
        restClient = *ref = NULL;
        return false;
    }
    *ref = restClient;
    return true;
}

bool RestClient::setup()
{
    Serial.println("INFO:  Initializing REST client");
    return true;
}

void RestClient::doGet(String url, RestResponse *response)
{
    httpClient.begin(url);
    Serial.printf("INFO:  GET %s\n", url.c_str());
    response->statusCode = httpClient.GET();
    String responseBody = httpClient.getString();
    response->body = StaticJsonDocument<1024>();
    deserializeJson(response->body, responseBody.c_str(), responseBody.length());
    Serial.printf("INFO:  Response %d => %s\n", response->statusCode, responseBody.c_str());
    httpClient.end();
}

void RestClient::doPost(String url, StaticJsonDocument<1024> message, RestResponse *response)
{
    char json[1024];
    const size_t length = serializeJson(message, json);
    Serial.printf("INFO:  POST %s => %s\n", url.c_str(), json);
    httpClient.begin(url);
    response->statusCode = httpClient.POST((uint8_t *) json, length);
    String responseBody = httpClient.getString();
    response->body = StaticJsonDocument<1024>();
    deserializeJson(response->body, responseBody.c_str(), responseBody.length());
    Serial.printf("INFO:  Response %d => %s\n", response->statusCode, responseBody.c_str());
    httpClient.end();
}

bool RestClient::publishBatteryVoltage(Device *device, const float voltage)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["voltage"] = voltage;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/battery/voltage", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishWeatherTemperature(Device *device, const float temperature)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["temperature"] = temperature;
    message["unit"] = "celcius";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/weather/temperature", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishWeatherHumidity(Device *device, const float humidity)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["humidity"] = humidity;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/weather/humidity", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishWeatherPressure(Device *device, const float pressure)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["pressure"] = pressure;
    message["unit"] = "pascals";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/weather/pressure", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishWeatherLight(Device *device, const float light)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["light"] = light;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/weather/light", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishTankDistance(Device *device, const uint8_t tank, const float distance)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["tank"] = tank;
    message["distance"] = distance;
    message["unit"] = "centimeters";
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/tank/distance", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}

bool RestClient::publishDripValveState(Device *device, const DripValveState state)
{
    StaticJsonDocument<1024> message;
    message["source"] = device->getMac().c_str();
    message["state"] = state;
    message["timestamp"] = networkTime->isotime();

    RestResponse response;
    doPost(baseUrl + "/rest/dripValve/state", message, &response);
    if (response.statusCode != 209) {
        Serial.printf("ERROR: Unexpected response from server: %d => %s\n",
                response.statusCode, response.body["message"].as<char *>());
        return false;
    }
    return true;
}
