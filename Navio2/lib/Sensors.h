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
#include "Navio2/MPU9250.h"
#include "Navio2/LSM9DS1.h"
#include "Navio2/Util.h"

// Global defined variables
#define G_SI 9.80665
#define PI   3.14159

class Sensors {
public:
    
    // Sensors data
    float ax_, ay_, az_;
    float gx_, gy_, gz_;
    float mx_, my_, mz_;
    float gyro_offset_[3]; 

    // Objects
    InertialSensor *imu_;    // IMU Sensor
    FILE * row_data_file_;   // File to store row data 
    
    // Checking variables
    bool IsIMUEnabled;
    bool IsIMUCalibrated;
    
    // Public methods
    Sensors();
    ~Sensors();
    bool createIMU(char *sensor_name);
    void update();
    void gyroCalibrate();
    
private:
    // Row sensor data
    long unsigned time_now_;
    
    // Private methods
    void updateIMU();
    void storeRowData();
    void getTime();
    void storeInfo();
};

#endif /* SENSORS_H */
