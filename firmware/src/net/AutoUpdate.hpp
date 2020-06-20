
#ifndef _AUTO_UPDATE_HPP
#define _AUTO_UPDATE_HPP


#include <ArduinoOTA.h>
#include <stdint.h>


class AutoUpdate {
protected:
    bool setup();

    static void onStart();
    static void onProgress(uint progress, uint total);
    static void onEnd();
    static void onError(ota_error_t error);

public:
    AutoUpdate();
    static bool get(AutoUpdate **ref);

    bool loop();
};


#endif
