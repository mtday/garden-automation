
#ifndef _GARDEN_AUTOMATION_SENSOR_GM_HPP
#define _GARDEN_AUTOMATION_SENSOR_GM_HPP


class SensorGM
{
public:
    SensorGM();

    bool setup();

    float readLight();
};


#endif
