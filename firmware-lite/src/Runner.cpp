
#include "Runner.hpp"


static Runner *runner;


Runner::Runner() {
    espNow = new EspNow();
    switch (Device::get()->getType()) {
        case DeviceTypeController:
            network = Network::get();
            ntpTime = NTPTime::get();
            messenger = Messenger::get();
            sensorBattery = NULL;
            sensorBME = NULL;
            sensorGM = NULL;
            break;
        case DeviceTypeWeather:
            network = NULL;
            ntpTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorBME = SensorBME::get();
            sensorGM = SensorGM::get();
            break;
        case DeviceTypeTankVolume:
            network = NULL;
            ntpTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorBME = SensorBME::get();
            sensorHCSR = SensorHCSR::get();
        case DeviceTypeDripValve:
            network = NULL;
            ntpTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorBME = NULL;
            sensorHCSR = NULL;
            break;
    }
}

Runner *Runner::get() {
    if (!runner) {
        runner = new Runner();
    }
    return runner;
}

bool Runner::setup() {
    Serial.println("INFO:  Initializing runner");
    if (network && !network->setup()) {
        return false;
    }
    if (ntpTime && !ntpTime->setup()) {
        return false;
    }
    if (espNow && !espNow->setup()) {
        return false;
    }
    if (sensorBattery && !sensorBattery->setup()) {
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

    switch (Device::get()->getType()) {
        case DeviceTypeController: {
            Serial.println("INFO:  Running controller setup");
            return true;
        }

        case DeviceTypeWeather: {
            Serial.println("INFO:  Running weather setup");
            const float voltage = sensorBattery->readVoltage();
            if (!espNow->sendBattery(voltage)) {
                return false;
            }
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

        case DeviceTypeTankVolume: {
            Serial.println("INFO:  Running tank volume setup");
            const float voltage = sensorBattery->readVoltage();
            if (!espNow->sendBattery(voltage)) {
                return false;
            }
            const float volume = sensorHCSR->readVolume();
            if (!espNow->sendTankVolume(volume)) {
                return false;
            }
            deepSleep(READING_TANK_VOLUME_INTERVAL);
            return true;
        }

        case DeviceTypeDripValve: {
            Serial.println("INFO:  Running drip valve setup");
            const float voltage = sensorBattery->readVoltage();
            if (!espNow->sendBattery(voltage)) {
                return false;
            }
            return true;
        }

        default:
            Serial.printf("ERROR: Unsupported device type: %c\n", Device::get()->getType());
            return false;
    }
}

bool Runner::loop() {
    if (network && !network->loop()) {
        return false;
    }
    if (messenger && !messenger->loop()) {
        return false;
    }
    return true;
}

void Runner::deepSleep(const ulong seconds) {
    const ulong microseconds = seconds * 1000 * 1000;
    esp_sleep_enable_timer_wakeup(microseconds);

    Serial.printf("INFO:  Deep sleep for %ld seconds\n", seconds);
    Serial.flush(); 
    esp_deep_sleep_start();
}

void Runner::restart() {
    Serial.println("INFO:  Restarting");
    esp_restart();
}
