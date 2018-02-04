#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "Navio2/MPU9250.h"
#include "Navio2/LSM9DS1.h"
#include "Navio2/Util.h"

#define G_SI 9.80665

// Structure

class IMU {
public:
    float gx, gy, gz;
    float ax, ay, az;
    float mx, my, mz;
    float offset[3];
    InertialSensor* is;

    IMU(char* sensor_name) {
    //---------------------------- Create IMU ----------------------------------
        if (!strcmp(sensor_name, "mpu")) {
            printf("Selected: MPU9250\n");
            imu = new MPU9250();
        } else if(!strcmp(sensor_name, "lsm")) {
            printf("Selected: LSM9DS1\n");
            imu = new LSM9DS1();
        } else {
            printf("Selected: MPU9250\n");
            imu = new MPU9250();
        }
        Calibrate();
    }

    void Calibrate() {
        int imax = 100;
        for (int i = 0; i < imax; i++) {
            is->update();
            is->read_gyroscope(&gx, &gy, &gz);
            offset[0] -= gx;
            offset[1] -= gy;
            offset[2] -= gz;
            usleep(1000);
        }
        offset[0] /= imax;
        offset[1] /= imax;
        offset[2] /= imax;
        printf("%f %f %f\n", offset[0], offset[1], offset[2]);
    }

    void update() {

        is->update();
        is->read_gyroscope(&gx, &gy, &gz);
        is->read_accelerometer(&ax, &ay, &az);
        is->read_magnetometer(&mx, &my, &mz);

    }
};

// Objects
IMU *imu;

//================================ Main loop ===================================

int main(int argc, char *argv[]) {
    // Timing data
    float dt, dtsumm = 0;
    //------------------------------ IMU setup ---------------------------------
    imu = new IMU("mpu");
    
    while (1) {
        //----------------------- Calculate delta time -------------------------
        usleep(100000);
        //--------------------------- Read Sensor ------------------------------
        IMU.update();
        //------------- Console and network output with a lowered rate ---------
        printf("%f %f %f\n", IMU.ax/G_SI, IMU.ay/G_SI, IMU.az/G_SI);
        
    }
    return 0;
}
