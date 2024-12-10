#include "IMUSensor.hpp"
#include <Arduino.h>

IMUSensor::IMUSensor() : myIMU() {}

void IMUSensor::init()
{
    myIMU.begin();
}

tuple<float, float, float> IMUSensor::getRawData()
{
    float x_val = myIMU.readFloatGyroX();
    float y_val = myIMU.readFloatGyroY();
    float z_val = myIMU.readFloatGyroZ();
    return {x_val, y_val, z_val};
}

tuple<float, float, float> IMUSensor::getData()
{
    float x_val = myIMU.readFloatGyroX() - imu_x_error;
    float y_val = myIMU.readFloatGyroY() - imu_y_error;
    float z_val = myIMU.readFloatGyroZ() - imu_z_error;
    return {x_val, y_val, z_val};
}

bool IMUSensor::calibrate(uint16_t iter_number)
{
    static int counter;
    auto [x, y, z] = getRawData();

    if (counter < iter_number)
    {
        counter++;
        imu_x_error += x;
        imu_y_error += y;
        imu_z_error += z;
    }
    else
    {

        imu_x_error = imu_x_error / iter_number;
        imu_y_error = imu_y_error / iter_number;
        imu_z_error = imu_z_error / iter_number;
        return true;
    }
    return false;
}

float IMUSensor::getYaw()
{
    auto [x, y, z] = getData();
    const float one_degree = 90.0;
    static float degrees;

    if (z > 0.7)
    {
        if (degrees < 180.0)
        {
            degrees += z / one_degree;
        }
        else
        {
            degrees *= -1;
        }
    }
    if (z < -0.7)
    {
        if (degrees > -180.0)
        {
            degrees -= abs(z) / one_degree;
        }
        else
        {
            degrees *= -1;
        }
    }

    return degrees;
}

float IMUSensor::getRoll()
{
    auto [x, y, z] = getData();
    const float one_degree = 90.0;
    static float degrees;

    if (x > 0.7)
    {
        if (degrees < 90.0)
        {
            degrees += x / one_degree;
        }
        else
        {
            degrees *= -1;
        }
    }

    if (x < -0.7)
    {
        if (degrees > -90.0)
        {
            degrees -= abs(x) / one_degree;
        }
        else
        {
            degrees *= -1;
        }
    }

    return degrees;
}