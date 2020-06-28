
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

int RestClient::doGet(String url, StaticJsonDocument<1024> response)
{
    httpClient.begin(url);
    const int status = httpClient.GET();
    String responseBody = httpClient.getString();
    deserializeJson(response, responseBody.c_str(), responseBody.length());
    httpClient.end();
    return status;
}

int RestClient::doPost(String url, StaticJsonDocument<1024> message)
{
    char json[1024];
    const size_t length = serializeJson(message, json);
    Serial.printf("INFO:  POST %s => %s\n", url.c_str(), json);
    httpClient.begin(url);
    const int status = httpClient.POST((uint8_t *) json, length);
    httpClient.end();
    return status;
}

bool RestClient::publishBatteryVoltage(Device *device, const float voltage)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["voltage"] = voltage;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing battery voltage to server");
    const int status = doPost(baseUrl + "/rest/battery/voltage", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherTemperature(Device *device, const float temperature)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["temperature"] = temperature;
    message["unit"] = "celcius";
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing weather temperature to server");
    const int status = doPost(baseUrl + "/rest/weather/temperature", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherHumidity(Device *device, const float humidity)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["humidity"] = humidity;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing weather humidity to server");
    const int status = doPost(baseUrl + "/rest/weather/humidity", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherPressure(Device *device, const float pressure)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["pressure"] = pressure;
    message["unit"] = "pascals";
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing atmospheric pressure to server");
    const int status = doPost(baseUrl + "/rest/weather/pressure", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherLight(Device *device, const float light)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["light"] = light;
    message["unit"] = "percent";
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing ambient light to server");
    const int status = doPost(baseUrl + "/rest/weather/light", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishTankDistance(Device *device, const uint8_t tank, const float distance)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["tank"] = tank;
    message["distance"] = distance;
    message["unit"] = "centimeters";
    message["timestamp"] = networkTime->isotime();

    Serial.printf("INFO:  Publishing tank %d distance to server\n", tank);
    const int status = doPost(baseUrl + "/rest/tank/distance", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishDripValveState(Device *device, const DripValveState state)
{
    StaticJsonDocument<1024> message;
    message["test"] = TEST_MODE;
    message["source"] = device->getMac().c_str();
    message["state"] = state;
    message["timestamp"] = networkTime->isotime();

    Serial.println("INFO:  Publishing drip valve state to server");
    const int status = doPost(baseUrl + "/rest/dripValve/state", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}
