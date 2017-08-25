#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"
#include "AHRS.hpp"
#include "SamplingTime.h"

#define G_SI 9.80665

// Structure
struct structIMU{
    float gx, gy, gz;
    float ax, ay, az;
    float mx, my, mz;
    float r, p, w;
};

struct structData{
    structIMU imu;
};

// Objects
InertialSensor *imu;
AHRS ahrs; // Mahony AHRS

// functions
InertialSensor* imuSetup(char *sensor_name);
void getIMU(structData* data, float dt);

#endif /* MAIN_HPP */

