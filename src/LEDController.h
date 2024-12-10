#pragma once
#include <FastLED.h>

class LEDController
{
private:
    int sensorValue = 0;
    int analog_pin = 36;
    int num_leds = 1;
    CRGB *leds;

public:
    LEDController();
    void initLEDs();
    void updateColor();
};
