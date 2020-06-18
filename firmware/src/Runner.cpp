
#include "Runner.hpp"


static Runner *runner;


Runner::Runner() {
    espNow = EspNow::get();
    switch (Device::get()->getType()) {
        case DeviceTypeController:
            network = Network::get();
            networkTime = NetworkTime::get();
            messenger = Messenger::get();
            sensorBattery = NULL;
            sensorWeather = NULL;
            sensorLight = NULL;
            sensorDistance = NULL;
            break;
        case DeviceTypeWeather:
            network = NULL;
            networkTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorWeather = SensorWeather::get();
            sensorLight = SensorLight::get();
            sensorDistance = NULL;
            break;
        case DeviceTypeTank:
            network = NULL;
            networkTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorWeather = SensorWeather::get();
            sensorLight = NULL;
            sensorDistance = SensorDistance::get();
        case DeviceTypeDripValve:
            network = NULL;
            networkTime = NULL;
            messenger = NULL;
            sensorBattery = SensorBattery::get();
            sensorWeather = NULL;
            sensorLight = NULL;
            sensorDistance = NULL;
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
    if (networkTime && !networkTime->setup()) {
        return false;
    }
    if (espNow && !espNow->setup()) {
        return false;
    }
    if (sensorBattery && !sensorBattery->setup()) {
        return false;
    }
    if (sensorWeather && !sensorWeather->setup()) {
        return false;
    }
    if (sensorLight && !sensorLight->setup()) {
        return false;
    }
    if (sensorDistance && !sensorDistance->setup()) {
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
            const float temperature = sensorWeather->readTemperature();
            const float humidity = sensorWeather->readHumidity();
            const float pressure = sensorWeather->readPressure();
            if (!espNow->sendWeather(temperature, humidity, pressure)) {
                return false;
            }
            const float light = sensorLight->readLight();
            if (!espNow->sendLight(light)) {
                return false;
            }
            deepSleep(WEATHER_SLEEP_PERIOD);
            return true;
        }

        case DeviceTypeTank: {
            Serial.println("INFO:  Running tank setup");
            const float voltage = sensorBattery->readVoltage();
            if (!espNow->sendBattery(voltage)) {
                return false;
            }
            const float distance = sensorDistance->readDistance();
            if (!espNow->sendTank(distance)) {
                return false;
            }
            deepSleep(TANK_SLEEP_PERIOD);
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

    ulong now = millis();
    if (sensorBattery && now - lastBatteryNotification > READING_BATTERY_INTERVAL) {
        lastBatteryNotification = now;
        const float voltage = sensorBattery->readVoltage();
        if (!espNow->sendBattery(voltage)) {
            return false;
        }
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
