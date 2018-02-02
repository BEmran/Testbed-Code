#ifndef IMU_HPP
#define IMU_HPP

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"

#define G_SI 9.80665
#define PI   3.14159

class IMU {
public:
    // Sensor data
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    float gyroOffset[3];

    // Objects
    InertialSensor *is; // Inertial Sensor

    // Methods

    IMU() {
    };

    void accCalibrate() {
    }; // TODO

    void magCalibrate() {
    }; // TODO

    //**************************************************************************
    // Create inertial sensor
    //**************************************************************************

    IMU(char *sensor_name) {
        if (!strcmp(sensor_name, "mpu")) {
            printf("Create inertial sensor as MPU9250\n");
            is = new MPU9250();
        } else if (!strcmp(sensor_name, "lsm")) {
            printf("Create inertial sensor as LSM9DS1\n");
            is = new LSM9DS1();
        } else {
            printf("Undefined sensor name.\n"
                    "No inertial sensor was created.\n"
                    "Please select: mpu or lsm\n");
            is = NULL;

        }
    }

    //**************************************************************************
    // Setup inertial sensor
    //**************************************************************************

    void imuSetup() {

        //if (!is->probe()) {
        //    printf("Sensor is not enable\n");
        //}

        // inertial sensor initialization
        is->initialize();

        // Calibrate gyro sensor
        gyroCalibrate();
        
    }

    //**************************************************************************
    // Calibrate gyro sensor
    //**************************************************************************

    void gyroCalibrate() {
        //--------------------------------------------------------------------------
        printf("Beginning Gyro calibration...\n");
        int maxCount = 500;
        for (int i = 0; i < maxCount; i++) {

            is->update(); // update inertial sensor
            is->read_gyroscope(&gx, &gy, &gz); // get gyro data 

            gx *= 180 / PI;
            gy *= 180 / PI;
            gz *= 180 / PI;
	    gyroOffset[0] += (-gx*0.0175);
	    gyroOffset[1] += (-gy*0.0175);
            gyroOffset[2] += (-gz*0.0175);
            
            //gyroOffset[0] += -gx;
            //gyroOffset[1] += -gy;
            //gyroOffset[2] += -gz;
            usleep(10000);
        }
        gyroOffset[0] /= maxCount;
        gyroOffset[1] /= maxCount;
        gyroOffset[2] /= maxCount;

        printf("Offsets are: %f %f %f\n", gyroOffset[0], gyroOffset[1], gyroOffset[2]);
    }

    //**************************************************************************
    // read IMU data and apply calibration offset
    //**************************************************************************

    void update() {

        // Read raw measurements from the inertial sensor
        is->update();
        is->read_gyroscope(&gx, &gy, &gz);
        is->read_accelerometer(&ax, &ay, &az);
        is->read_magnetometer(&mx, &my, &mz);

        // Scale raw measurements 
        ax /= G_SI;
        ay /= G_SI;
        az /= G_SI;
        gx *= 180 / PI;
        gy *= 180 / PI;
        gz *= 180 / PI;

        // TODO: Apply calibration offset
        //gx -= gyroOffset[0];
	//gy -= gyroOffset[1];
	//gz -= gyroOffset[2];
        // TODO: Apply rotation for x front y left and z up
    }

};

#endif /* IMU_HPP */