/* 
 * File:   Sensors.h
 * Author: Bara Emran
 *
 * Created on February 3, 2018, 11:13 AM
 */

#ifndef SENSORS_H
#define SENSORS_H

// general libraries
#include <stdio.h>  // file, printf
#include <sys/time.h> // time

// Navio2 libraries
#include "Navio/Common/MPU9250.h" // IMU mpu chip
#include "Navio/Navio2/LSM9DS1.h" // IMU lsm chip
#include "Navio/Common/Util.h"  
#include "Navio/Common/MS5611.h" // Pressure And temperature chip

// Global defined variables
#define G_SI 9.80665
#define PI   3.14159

class Sensors {
public:
    
    // Sensors data
    float ax_, ay_, az_;    // Gyro
    float gx_, gy_, gz_;    // Accelerometer
    float mx_, my_, mz_;    // Magnetometer
    float gyro_offset_[3];  // offset data of gyro sensor
    float temp_;            // Temperature
    float pres_;            // Pressure

    // Objects
    InertialSensor *imu_;    // IMU sensor
    MS5611 *barometer_;       // Barometer sensor
    FILE * row_data_file_;   // File to store row data 
    FILE * notes_file_;      // File to store notes
    // Checking variables
    bool IsIMUEnabled;
    bool IsIMUCalibrated;
    
    // Public methods
    Sensors();
    ~Sensors();
    bool createIMU(char *sensor_name);
    bool createBarometer(char *sensor_name);
    void updateAll();
    void updateIMU();
    void updateBarometer();
    void gyroCalibrate();
    void displayData();
    
private:
    // Row sensor data
    long unsigned time_now_;
    
    // Private methods
    void getTime();
    void storeData();
    void storeInfo();
};

#endif /* SENSORS_H */