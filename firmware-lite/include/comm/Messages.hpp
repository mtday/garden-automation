
#ifndef _GARDEN_AUTOMATION_COMM_MESSAGES_HPP
#define _GARDEN_AUTOMATION_COMM_MESSAGES_HPP


typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float light;
} WeatherData;


typedef struct
{
    float volume;
} TankVolumeData;


typedef struct
{
    bool status;
} TankValveControl;


#endif
