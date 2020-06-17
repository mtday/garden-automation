
#include "Device.hpp"


static Device *device;


Device::Device(Mac mac, DeviceType type)
{
    Device::mac = mac;
    Device::type = type;
    Serial.printf("INFO:  Device type %c has mac %s\n", type, mac.c_str());

    espNow = new EspNow();
    switch (type) {
        case Controller:
            network = Network::get();
            ntpTime = NTPTime::get();
            messenger = Messenger::get();
            break;
        case Weather:
            sensorBME = SensorBME::get();
            sensorGM = SensorGM::get();
            break;
        case TankVolume:
            sensorBME = SensorBME::get();
            sensorHCSR = SensorHCSR::get();
        case TankValve:
            break;
    }
}

Device *Device::get()
{
    if (device) {
        return device;
    }

    Mac mac;
    DeviceType type;
    if (mac == DEVICE_AUTOMATION_CONTROL) {
        type = Controller;
    } else if (mac == DEVICE_GARDEN_WEATHER) {
        type = Weather;
    } else if (mac == DEVICE_TANK_VALVE) {
        type = TankValve;
    } else {
        type = TankVolume;
    }
    device = new Device(mac, type);
    return device;
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
    if (network && !network->setup()) {
        return false;
    }
    if (ntpTime && !ntpTime->setup()) {
        return false;
    }
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
            return true;
        }

        default:
            Serial.printf("ERROR: Unsupported device type: %c\n", type);
            return false;
    }
}

bool Device::loop()
{
    if (network && !network->loop()) {
        return false;
    }
    if (messenger && !messenger->loop()) {
        return false;
    }
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

void Device::restart()
{
    Serial.println("INFO:  Restarting");
    esp_restart();
}
