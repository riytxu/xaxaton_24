#include <chrono>
#include <tuple>
#include "IMUSensor.hpp"

using namespace std::chrono;
using namespace std;

class PD
{
private:
    float kp = 0.0;
    float kd = 0.0;
    float prev_error = 0.0;
    float prev_time = 0;

public:
    PD(float p_gain, float d_gain);
    float get_output(float error);
};

class Regulators
{
private:
    IMUSensor imu;
    PD yaw_regulator;
    PD roll_regulator;

    float clamp_to180(float angle);
    float clamp(float v, float max_v, float min_v);

public:
    Regulators(float p1, float d1, float p2, float d2);
    float keepYaw(float angle);
    float keepRoll(float angle);
};