#include <Arduino.h>
#include "MotorController.h"
#include "LEDController.h"
#include "PR.h"
#include "PD.hpp"

#define DEBUG 0

#define P1 0.5 // TODO: Коэф регулятора по Курс
#define D1 0.5 // TODO: Коэф регулятора по Курс
#define P2 0.5 // TODO: Коэф регулятора по Крен
#define D2 0.5 // TODO: Коэф регулятора по Крен

#define GOR_MOTOR_STATIC 80 // TODO: Процентаная постоянная тяга на гор.движ
#define STEPS 3

MotorController motors;
LEDController ledController;
PR pr;
Regulators reg(P1, D1, P2, D2);

float yaw = 0.0f;
float roll = 0.0f;

int depth = 60; // TODO: Процентаная постоянная тяга для грубины
int side = 60;  // TODO: Процентаная постоянная тяга на лага

time_t timeshtamp;
time_t currentTime;
time_t time_period = 15000; // TODO: Время на нахождение препятствия

bool pr_value = false;
bool pr_prev_value = false;

int loop_counter = 0;
bool time_status = false;
bool pr_status = false;

enum Cycle
{
  RightTop,
  RightBot,
  LeftBot,
  LeftTop,
  ALL_STATUS
};
Cycle cycle_status = RightTop;

void cycleFunction(int side_direction, int depth_direction)
{
  side *= side_direction;
  depth *= depth_direction;
}

void setup()
{
  if (DEBUG)
  {
    Serial.begin(115200);
  }

  ledController.initLEDs();
  motors.begin();
  pr.init();
  timeshtamp = millis();
}

void loop()
{
  yaw = reg.keepYaw(0);  // Статическое выравнивание по Курсу
  roll = reg.keepYaw(0); // Статическое выравнивание по Крену

  currentTime = millis() - timeshtamp;
  pr_value = pr.getStatus();

  time_status = currentTime >= time_period;
  pr_status = pr_value == true && pr_prev_value == false;

  if (time_status || pr_status)
  {
    loop_counter++;
    timeshtamp = millis();
  }

  if (loop_counter >= STEPS)
  {
    depth = 100;
  }
  else
  {
    switch (cycle_status)
    {
    case Cycle::RightTop:
      cycleFunction(1, -1);
      break;
      ;
    case Cycle::RightBot:
      cycleFunction(-1, -1);
      break;
      ;
    case Cycle::LeftBot:
      cycleFunction(-1, 1);
      break;
      ;
    case Cycle::LeftTop:
    default:
      cycleFunction(1, 1);
      break;
    }

    if (currentTime >= (time_period / 4) + (cycle_status * (time_period / 4)))
    {
      cycle_status = static_cast<Cycle>((cycle_status + 1) % Cycle::ALL_STATUS);
    }
  }

  motors.setMotorPower(0, GOR_MOTOR_STATIC + yaw);
  motors.setMotorPower(1, GOR_MOTOR_STATIC - yaw);
  motors.setMotorPower(2, 100);

  motors.setMotorPower(3, depth + roll);
  motors.setMotorPower(4, depth - roll);
  motors.setMotorPower(5, side);

  pr_prev_value = pr_value;
  ledController.updateColor(); // Просто обновляем в цикле
}