/* 
 * File:   Imu.cpp
 * Author: Bara Emran
 *
 * Created on February 3, 2018, 11:58 AM
 */

#include <stdio.h>
#include <sys/time.h>
#include "Sensors.h"

unsigned long previoustime, currenttime;
float getdt(float Hz);

//******************************************************************************
// Print help message
//******************************************************************************

void print_help() {
    printf("Possible parameters:\nSensor selection: -i [sensor name]\n");
    printf("Sensors names: mpu is MPU9250, lsm is LSM9DS1\nFor help: -h\n");
    printf("If you want to visualize IMU data on another machine,\n");
}

//******************************************************************************
// Main function
//******************************************************************************

int main(int argc, char *argv[]) {
    int parameter;
    char *sensor_name;

    if (check_apm()) {
        return 1;
    }

    if (argc < 2) {
        printf("Enter parameter\n");
        print_help();
        return 0;
    }

    // prevent the error message
    opterr = 0;
    while ((parameter = getopt(argc, argv, "i:h")) != -1) {
        switch (parameter) {
            case 'i': sensor_name = optarg;
                break;
            case 'h': print_help();
                return 0;
            case '?': printf("Wrong parameter.\n");
                print_help();
                return 0;
        }
    }

    // Initialize imu sensor
    Sensors *sensors = new Sensors();
    if (!sensors->createIMU(sensor_name)) {
        printf("Wrong sensor name. Select: mpu or lsm\n");
        return EXIT_FAILURE;
    }
    
    // Calibrate gyro sensor
    sensors->gyroCalibrate();

    // Timing data
    float dtsumm = 0;
    float dt = getdt(1300.0);
    while (1) {
        // Calculate delta time 
        dt = getdt(1300.0);

        // update imu data
        sensors->update();

        //------------- Console and network output with a lowered rate ------------
        dtsumm += dt;
        if (dtsumm > 0.05) {
            dtsumm = 0;
            // Console output
            fprintf("%d %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f\n",
                    int(1 / dt),
                    sensors.gx_, sensors.gy_, sensors.gz_,
                    sensors.ax_, sensors.ay_, sensors.az_,
                    sensors.mx_, sensors.my_, sensors.mz_);
        }
    }
}

float getdt(float Hz) {

    // Timing data
    struct timeval tv;
    float dt;

    // Calculate delta time 
    gettimeofday(&tv, NULL);
    currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
    dt = (currenttime - previoustime) / 1000000.0;

    // Sleep if the dt is smaller than the required 1/frequency
    if (dt < 1 / Hz) {
        usleep((1 / Hz - dt)*1000000);
    }

    // Calculate delta time after sleep
    gettimeofday(&tv, NULL);
    currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
    dt = (currenttime - previoustime) / 1000000.0;

    // store current time
    previoustime = currenttime;

    return dt;
}