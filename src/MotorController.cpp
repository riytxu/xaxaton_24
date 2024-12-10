#include "MotorController.h"

void MotorController::begin()
{
    for (int i = 0; i < 3; i++)
    {
        pinMode(motors[i].ain1, OUTPUT);
        pinMode(motors[i].ain2, OUTPUT);
        pinMode(motors[i].bin1, OUTPUT);
        pinMode(motors[i].bin2, OUTPUT);
        pinMode(motors[i].pwma, OUTPUT);
        pinMode(motors[i].pwmb, OUTPUT);
    }
}

void MotorController::setMotor(int driver, char channel, int speed)
{
    if (driver < 1 || driver > 3 || (channel != 'A' && channel != 'B'))
    {
        return; // Неверный драйвер или канал
    }

    MotorPins &pins = motors[driver - 1];
    int pwmPin = (channel == 'A') ? pins.pwma : pins.pwmb;
    int in1 = (channel == 'A') ? pins.ain1 : pins.bin1;
    int in2 = (channel == 'A') ? pins.ain2 : pins.bin2;

    speed = constrain(speed, -255, 255); // Ограничиваем диапазон скорости

    if (speed > 0)
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(pwmPin, speed);
    }
    else if (speed < 0)
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(pwmPin, -speed);
    }
    else
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(pwmPin, 0);
    }
}

void MotorController::setMotorPower(int motor, int power)
{
    int driver = (motor / 2) + 1;
    char channel = (motor % 2 == 0) ? 'A' : 'B';
    power = map(power, -100, 100, -255, 255);
    setMotor(driver, channel, power);
}

tuple<double, double, double> MotorController::calcVerticalPower(double angle, int speed)
{
    double power1, power2, power3;

    power1 = speed * cos(degToRad(angle - motor_angles[0]));
    power2 = speed * sin(degToRad(angle - motor_angles[1]));
    power3 = speed * cos(degToRad(angle - motor_angles[2]));

    return {power1, power2, power3};
}

double MotorController::degToRad(double degrees)
{
    return degrees * M_PI / 180.0;
}