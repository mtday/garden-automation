
#include "Device.hpp"
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
    char mac[18];
    strncpy(mac, Device::get()->getMac().c_str(), 18);

    // add common fields to the message
    message["device"] = mac;
    message["test"] = TEST_MODE;
    message["timestamp"] = networkTime->isotime().c_str();

    char json[1024];
    size_t length = serializeJson(message, json);

    httpClient.begin(url);
    const int status = httpClient.POST((uint8_t *) json, length);
    httpClient.end();
    return status;
}

bool RestClient::publishBatteryVoltage(const float voltage)
{
    StaticJsonDocument<1024> message;
    message["voltage"] = voltage;
    message["unit"] = "percent";

    Serial.println("INFO:  Publishing battery voltage to server");
    const int status = doPost(baseUrl + "/rest/battery/voltage", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherTemperature(const float temperature)
{
    StaticJsonDocument<1024> message;
    message["temperature"] = temperature;
    message["unit"] = "celcius";

    Serial.println("INFO:  Publishing weather temperature to server");
    const int status = doPost(baseUrl + "/rest/weather/temperature", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherHumidity(const float humidity)
{
    StaticJsonDocument<1024> message;
    message["humidity"] = humidity;
    message["unit"] = "percent";

    Serial.println("INFO:  Publishing weather humidity to server");
    const int status = doPost(baseUrl + "/rest/weather/humidity", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherPressure(const float pressure)
{
    StaticJsonDocument<1024> message;
    message["pressure"] = pressure;
    message["unit"] = "pascals";

    Serial.println("INFO:  Publishing atmospheric pressure to server");
    const int status = doPost(baseUrl + "/rest/weather/pressure", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishWeatherLight(const float light)
{
    StaticJsonDocument<1024> message;
    message["light"] = light;
    message["unit"] = "percent";

    Serial.println("INFO:  Publishing ambient light to server");
    const int status = doPost(baseUrl + "/rest/weather/light", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishTankDistance(const float distance)
{
    StaticJsonDocument<1024> message;
    message["distance"] = distance;
    message["unit"] = "centimeters";

    Serial.printf("INFO:  Publishing tank distance to server\n");
    const int status = doPost(baseUrl + "/rest/tank/distance", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}

bool RestClient::publishDripValveState(const DripValveState state)
{
    StaticJsonDocument<1024> message;
    message["state"] = state;

    Serial.println("INFO:  Publishing drip valve state to server");
    const int status = doPost(baseUrl + "/rest/dripValve/state", message);
    if (status != 200) {
        Serial.printf("ERROR: Unexpected response from server: %d\n", status);
        return false;
    }
    return true;
}
