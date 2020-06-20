
#include <Arduino.h>
#include "net/AutoUpdate.hpp"


static AutoUpdate *autoUpdate;


AutoUpdate::AutoUpdate() {
}

bool AutoUpdate::get(AutoUpdate **ref) {
    if (autoUpdate) {
        *ref = autoUpdate;
        return true;
    }
    autoUpdate = new AutoUpdate();
    if (!autoUpdate->setup()) {
        autoUpdate = *ref = NULL;
        return false;
    }
    *ref = autoUpdate;
    return true;
}

bool AutoUpdate::setup() {
    ArduinoOTA
        .onStart(AutoUpdate::onStart)
        .onProgress(AutoUpdate::onProgress)
        .onEnd(AutoUpdate::onEnd)
        .onError(AutoUpdate::onError)
        .begin();
    return true;
}

bool AutoUpdate::loop() {
}

void AutoUpdate::onStart() {
    const String type = ArduinoOTA.getCommand() == U_FLASH ? "flash" : "spffs";
    Serial.printf("INFO:  Starting update %s\n", type);
}

void AutoUpdate::onProgress(uint progress, uint total) {
    Serial.printf("INFO:  Update progress %.02f\n", progress * 100.0 / total);
}

void AutoUpdate::onEnd() {
    Serial.println("INFO:  Update completed");
}

void AutoUpdate::onError(ota_error_t error) {
    const char *errorMessage;
    switch (error) {
        case OTA_AUTH_ERROR:
            errorMessage = "ERROR: Update authentication error";
            break;
        case OTA_BEGIN_ERROR:
            errorMessage = "ERROR: Update error during initialization";
            break;
        case OTA_CONNECT_ERROR:
            errorMessage = "ERROR: Connection failed during update";
            break;
        case OTA_RECEIVE_ERROR:
            errorMessage = "ERROR: Failed to receive data during update";
            break;
        case OTA_END_ERROR:
            errorMessage = "ERROR: Failed to finalize update";
            break;
    }
    Serial.println(errorMessage);
}
