#ifndef AHRS_H
#define AHRS_H

#include <cmath>
#include <stdio.h>

#define PI   3.14159

class AHRS{
private:
	float q0, q1, q2, q3;
	float twoKi;
	float twoKp;
	float integralFBx, integralFBy, integralFBz;
public:
    AHRS();

    void update( float dt, float gx, float gy, float gz, float ax, float ay, float az,float mx = 0.0, float my  = 0.0, float mz = 0.0);
    void getEuler(float* roll, float* pitch, float* yaw);

    float invSqrt(float x);
    float getW();
    float getX();
    float getY();
    float getZ();
};

#endif // AHRS_h
