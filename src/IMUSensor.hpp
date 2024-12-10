#pragma once

#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <tuple>

using namespace std;

class IMUSensor{
private:
    LSM6DS3 myIMU;
    float imu_x_error = 2.58;
    float imu_y_error = 0;
    float imu_z_error = -1.38;
public:
    IMUSensor();
    void init();
    tuple<float, float, float> getRawData();
    tuple<float, float, float> getData();
    bool calibrate(uint16_t iter_number = 100);
    float getYaw();
    float getRoll();
};