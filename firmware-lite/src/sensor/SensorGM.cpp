
#include "sensor/SensorGM.hpp"


SensorGM::SensorGM()
{
}

bool SensorGM::setup()
{
    Serial.println("INFO:  Initializing GM sensor");
    return true;
}

float SensorGM::readLight()
{
    const float light = 0.0;
    Serial.printf("INFO:  GM sensor read light: %f\n", light);
    return light;
}
