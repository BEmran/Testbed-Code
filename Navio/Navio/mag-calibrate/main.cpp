/* 
 * File:   main.cpp
 * Author: Bara Emran
 *
 * Created on February 3, 2018, 11:58 AM
 */

#include <stdio.h>
#include <sys/time.h>
#include "../lib/Sensors.h"

unsigned long previoustime, currenttime;
float getdt(float Hz);
void avgMag(Sensors* sensors);

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
    // Check if APM is busy: return false if free 
    if (check_apm()) {
        return 1;
    }
    // Print help message for less than two parameters 
    if (argc < 2) {
        printf("Enter parameter\n");
        print_help();
        return 0;
    }

    // Prevent the error message
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
   
    printf("Start magnetometer calibration procedure\n");
    while(1){
        printf("Place the autopilot in the required configuration and press enter\n");
        getchar();
        avgMag(sensors);
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

//**************************************************************************
// Find average of a single axis
//**************************************************************************

void avgMag(Sensors* sensors) {
    //--------------------------------------------------------------------------
    printf("Beginning calibration...\n");
    float maxCount = 500.0;
    float sumx = 0, sumy = 0, sumz = 0;
    for (int i = 0; i < int(maxCount); i++) {
        sensors->updateIMU();
        sumx = sumx + sensors->mx_;
        sumy = sumy + sensors->my_;
        sumz = sumz + sensors->mz_;
        usleep(10000);
    }

    printf("average data of are x = %10.5f y = %10.5f z = %10.5f \n", sumx / maxCount, sumy / maxCount, sumz / maxCount);
}
