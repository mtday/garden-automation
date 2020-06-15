
#ifndef _GARDEN_AUTOMATION_GM_SENSOR_H
#define _GARDEN_AUTOMATION_GM_SENSOR_H


using namespace std;


class GmSensor
{
public:
    GmSensor();

    bool setup();

    float readLight();
};


#endif
