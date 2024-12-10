#include "PD.hpp"

PD::PD(float p_gain, float d_gain)
{
    kp = p_gain;
    kd = d_gain;
}

float PD::get_output(float error)
{
    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();
    int curr_time = dtn.count();

    float output = kp * error + kd / (curr_time - prev_time) * (error - prev_error);

    prev_time = curr_time;
    prev_error = error;
    return output;
}

Regulators::Regulators(float p1, float d1, float p2, float d2) : yaw_regulator(p1, d1), roll_regulator(p2, d2)
{
    imu.init();
}

float Regulators::clamp(float v, float max_v, float min_v)
{
    if (v > max_v)
        return max_v;
    if (v < min_v)
        return min_v;
    return v;
}

float Regulators::clamp_to180(float angle)
{
    if (angle > 180.0)
        return angle - 360;
    if (angle < -180)
        return angle + 360;
    return angle;
}

float Regulators::keepYaw(float angle)
{
    float error = imu.getYaw() - angle;
    error = clamp_to180(error);
    float output = clamp(yaw_regulator.get_output(error), 100, -100);
    return output;
}

float Regulators::keepRoll(float angle)
{
    float error = imu.getRoll() - angle;
    float output = clamp(yaw_regulator.get_output(error), 100, -100);
    return output;
}