#pragma once
#include <tuple>
#include <Arduino.h>

using namespace std;
class MotorController
{
private:
    struct MotorPins
    {
        int pwma, ain1, ain2, bin1, bin2, pwmb;
    };

    MotorPins motors[3] = {
        // Драйвер 1 (моторы 1 и 2)
        {4, 5, 13, 14, 15, 16},
        // Драйвер 2 (моторы 3 и 4)
        {17, 18, 19, 21, 22, 23},
        // Драйвер 3 (моторы 5 и 6)
        {25, 26, 27, 32, 33, 12}};

    const double motor_angles[3] = {60.0, 180.0, 300.0};

public:
    void begin();
    void setMotor(int driver, char channel, int speed);
    void setMotorPower(int motor, int power);
    tuple<double, double, double> calcVerticalPower(double angle, int speed);
    double degToRad(double degrees);
};
