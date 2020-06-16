
#include "Device.hpp"


static Device *device;


Device *Device::getDevice()
{
    if (device) {
        return device;
    }

    Mac mac;
    if (mac == DEVICE_TANK_1_VOLUME ||
        mac == DEVICE_TANK_2_VOLUME ||
        mac == DEVICE_TANK_3_VOLUME ||
        mac == DEVICE_TANK_4_VOLUME) {
        device = new Device(mac, TankVolume);
    } else if (mac == DEVICE_TANK_VALVE) {
        device = new Device(mac, TankValve);
    } else if (mac == DEVICE_GARDEN_WEATHER) {
        device = new Device(mac, Weather);
    } else if (mac == DEVICE_AUTOMATION_CONTROL) {
        device = new Device(mac, Controller);
    } else {
        Serial.printf("ERROR: Unrecognized device mac address: %s\n", mac.c_str());
        device = NULL;
    }
    return device;
}

Device::Device(Mac mac, DeviceType type)
{
    Device::mac = mac;
    Device::type = type;
    Serial.printf("INFO:  Device type %c has mac %s\n", type, mac.c_str());

    espNow = new EspNow();
    switch (type) {
        case Controller:
            break;
        case Weather:
            sensorBME = new SensorBME();
            sensorGM = new SensorGM();
            break;
        case TankVolume:
            sensorBME = new SensorBME();
            sensorHCSR = new SensorHCSR(sensorBME);
        case TankValve:
            break;
    }
}

Mac Device::getMac() const
{
    return mac;
}

DeviceType Device::getType() const
{
    return type;
}

bool Device::setup()
{
    Serial.println("INFO:  Initializing device");
    if (espNow && !espNow->setup()) {
        return false;
    }
    if (sensorBME && !sensorBME->setup()) {
        return false;
    }
    if (sensorGM && !sensorGM->setup()) {
        return false;
    }
    if (sensorHCSR && !sensorHCSR->setup()) {
        return false;
    }

    switch (type) {
        case Controller: {
            Serial.println("INFO:  Running controller device setup");
            // TODO
            return true;
        }

        case Weather: {
            Serial.println("INFO:  Running weather device setup");
            const float temperature = sensorBME->readTemperature();
            const float humidity = sensorBME->readHumidity();
            const float pressure = sensorBME->readPressure();
            const float light = sensorGM->readLight();
            if (!espNow->sendWeather(temperature, humidity, pressure, light)) {
                return false;
            }
            deepSleep(READING_WEATHER_INTERVAL);
            return true;
        }

        case TankVolume: {
            Serial.println("INFO:  Running tank volume device setup");
            const float volume = sensorHCSR->readVolume();
            if (!espNow->sendTankVolume(volume)) {
                return false;
            }
            deepSleep(READING_TANK_VOLUME_INTERVAL);
            return true;
        }

        case TankValve: {
            Serial.println("INFO:  Running tank valve device setup");
            // TODO
            return true;
        }

        default:
            Serial.printf("ERROR: Unsupported device type: %c\n", type);
            return false;
    }
}

bool Device::loop()
{
    return true;
}

void Device::deepSleep(const ulong seconds)
{
    const ulong microseconds = seconds * 1000 * 1000;
    esp_sleep_enable_timer_wakeup(microseconds);

    Serial.printf("INFO:  Deep sleep for %ld seconds\n", seconds);
    Serial.flush(); 
    esp_deep_sleep_start();
}

void Device::weather(Mac source, const float temperature, const float humidity, const float pressure, const float light)
{
    Serial.printf("INFO:  Received weather from %s: %f, %f, %f, %f\n",
            source.c_str(), temperature, humidity, pressure, light);
    if (type == Controller) {
        // TODO
    } else {
        Serial.println("ERROR: Unexpectedly received weather data message");
    }
}

void Device::tankVolume(Mac source, const float volume)
{
    Serial.printf("INFO:  Received tank volume from %s: %f\n", source.c_str(), volume);
    if (type == Controller) {
        // TODO
    } else {
        Serial.println("ERROR: Unexpectedly received tank volume data message");
    }
}

void Device::tankValve(Mac source, const bool status)
{
    Serial.printf("INFO:  Received tank valve control from %s: %s\n", source.c_str(), status ? "open" : "close");
    if (type == Controller) {
        // TODO
    } else if (type == TankValve) {
        // TODO
    } else {
        Serial.println("ERROR: Unexpectedly received tank valve control message");
    }
}
