#include <Arduino.h>
#include "LEDController.h"

LEDController::LEDController()
{
    leds = new CRGB[num_leds];
}

void LEDController::initLEDs()
{
    pinMode(analog_pin, INPUT_PULLUP);
    FastLED.addLeds<WS2812B, 2, RGB>(leds, num_leds);
    FastLED.show();
}

void LEDController::updateColor()
{
    sensorValue = analogRead(analog_pin);
    static CRGB lastColor = CRGB::Black;

    CRGB colors[] = {CRGB::Red, CRGB::Blue, CRGB::Yellow, CRGB::Green};
    int thresholds[] = {1023, 2046, 3069};

    int colorIndex = 0;
    for (int i = 0; i < 3; i++)
    {
        if (sensorValue <= thresholds[i])
        {
            colorIndex = i + 1;
            break;
        }
    }

    CRGB newColor = colors[colorIndex];

    // Если новый цвет отличается от последнего, обновляем светодиод
    if (newColor != lastColor)
    {
        leds[0] = newColor;
        FastLED.show();
        lastColor = newColor;
    }
}
