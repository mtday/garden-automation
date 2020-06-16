
#ifndef _GARDEN_AUTOMATION_SENSOR_GM_SENSOR_HPP
#define _GARDEN_AUTOMATION_SENSOR_GM_SENSOR_HPP


class GmSensor
{
public:
    GmSensor();

    bool setup();

    float readLight();
};


#endif
