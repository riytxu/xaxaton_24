#pragma once

#include <Arduino.h>

class PR
{
private:
    int sensorValue = 0;
    int analog_pin = 0;
    int porogValue = 700; // TODO: Значение срабатывания фоторезистора

public:
    void init()
    {
        pinMode(analog_pin, INPUT_PULLUP);
    }

    bool getStatus()
    {
        sensorValue = analogRead(analog_pin);
        return sensorValue >= porogValue;
    }
};