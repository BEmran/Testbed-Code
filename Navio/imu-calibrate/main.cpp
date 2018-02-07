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
float avgAcc(Sensors* sensors, int axis);

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
    
    FILE* file = fopen("acc_calib.txt","w");
    
    float ax_pos_avg,ay_pos_avg,az_pos_avg;
    float ax_neg_avg,ay_neg_avg,az_neg_avg;
   
    printf("Start accelerometer calibration procedure\n");
    printf("Place the autopilot >>>> Flat <<<< and press enter\n");
    getchar();
    az_neg_avg = avgAcc(sensors, 2);
    
    printf("Place the autopilot on its >>>> RIGHT<<<< and press enter\n");
    getchar();
    ay_pos_avg = avgAcc(sensors, 1);

    printf("Place the autopilot on its >>>> LEFT <<<< and press enter\n");
    getchar();
    ay_neg_avg = avgAcc(sensors, 1);

    printf("Place the autopilot facing >>>> DOWN <<<< and press enter\n");
    getchar();
    ax_pos_avg = avgAcc(sensors, 0);

    printf("Place the autopilot facing >>>> UP <<<< and press enter\n");
    getchar();
    ax_neg_avg = avgAcc(sensors, 0);

    printf("Place the autopilot on its >>>> BACK <<<< and press enter\n");
    getchar();
    az_pos_avg = avgAcc(sensors, 2);  
    
    float ax_mid,ay_mid,az_mid;
    float ax_sen,ay_sen,az_sen;
    
    ax_mid = (ax_pos_avg + ax_neg_avg)/2.0;
    ay_mid = (ay_pos_avg + ay_neg_avg)/2.0;
    az_mid = (az_pos_avg + az_neg_avg)/2.0;
    ax_sen = (ax_pos_avg - ax_neg_avg)/2.0;
    ay_sen = (ay_pos_avg - ay_neg_avg)/2.0;
    az_sen = (az_pos_avg - az_neg_avg)/2.0;        
    
    printf("-------------------------------------------------------------\n");
    printf("Finish calibration, Here is the result:\n");
    printf("Middle      value for x-axis:%+5.5f  z-axis:%+5.5f  z-axis:%+5.5f\n",
            ax_mid,ay_mid,az_mid);
    printf("Sensitivity value for x-axis:%+5.5f  z-axis:%+5.5f  z-axis:%+5.5f\n",
            ax_sen,ax_sen,ax_sen);
    printf("-------------------------------------------------------------\n");

    fprintf (file,"zero %+5.5f %+5.5f %+5.5f\n",ax_mid,ay_mid,az_mid);
    fprintf (file,"sen  %+5.5f %+5.5f %+5.5f\n",ax_sen,ay_sen,az_sen);
    fclose(file);
    float dt,dtsumm=0;
    while(1){            
        // Calculate delta time 
        dt = getdt(400.0);

        // update imu data
        sensors->updateIMU();

        //------------- Console and network output with a lowered rate ------------
        dtsumm += dt;
        if (dtsumm > 0.05) {
            dtsumm = 0;
            // Console output
            printf("%d %+5.5f %+5.5f %+5.5f %+5.5f %+5.5f %+5.5f \n",
                    int(1 / dt),
                    sensors->ax_, sensors->ay_, sensors->az_,
                    (sensors->ax_-ax_mid)/ax_sen,
		    (sensors->ay_-ay_mid)/ay_sen,
                    (sensors->az_-az_mid)/az_sen);
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

//**************************************************************************
// Find average of a single axis
//**************************************************************************

float avgAcc(Sensors* sensors, int axis) {
    //--------------------------------------------------------------------------
    printf("Beginning axis-%d calibration...\n", axis);
    float maxCount = 500.0;
    float sum = 0;
    for (int i = 0; i < int(maxCount); i++) {
        sensors->updateIMU();
        switch (axis) {
            case(0):
                sum = sum + sensors->ax_;
                break;
            case(1):
                sum = sum + sensors->ay_;
                break;
            case(2):
                sum = sum + sensors->az_;
                break;
        }
        usleep(10000);
    }
    float avg = sum / maxCount;

    printf("average data of axis-%d is %5.5f \n", axis, avg);
    
    return avg;
}
