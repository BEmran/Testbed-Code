/*
 * File:   Sensors.cpp
 * Author: Bara Emran
 * 
 * Created on February 3, 2018, 11:13 AM
 */

#include "Sensors.h"

Sensors::Sensors() {
    // Initialize default values
    imu_ = NULL;
    IsIMUEnabled = false;
    IsIMUCalibrated = false;
    gyro_offset_ [0] = 0.0;
    gyro_offset_ [1] = 0.0;
    gyro_offset_ [2] = 0.0;

    // Create a file to store the row data
    row_data_file_ = fopen("row_data.txt", "w");
    printf("Start storing the data in the file \"row_data.txt\"\n");

    getTime(); // Initialize time variables the current time 
    fprintf(row_data_file_, "Row data started in the following order:\n"
            "time, gx, gy, gz, ax, ay, az, mx, my, mz\n");
}

Sensors::~Sensors() {
    printf("Delete sensors object\n");
            delete this;
}
//**************************************************************************
// Create imu sensor
//**************************************************************************

bool Sensors::createIMU(char *sensor_name) {
    // Create an imu sensor if it's a valid name
    if (!strcmp(sensor_name, "mpu")) {
        printf("Create IMU sensor as MPU9250\n");
        imu_ = new MPU9250();
    } else if (!strcmp(sensor_name, "lsm")) {
        printf("Create IMU sensor as LSM9DS1\n");
        imu_ = new LSM9DS1();
    } else {
        printf("Undefined sensor name.\n"
                "No IMU sensor was created.\n"
                "Please select: mpu or lsm\n");
        imu_ = NULL;
        return false;
    }
    // Check if imu sensor is enabled
    imu_->initialize();

    if (imu_->probe()) {
        printf("IMU sensor is enabled\n");
        IsIMUEnabled = true;
    } else {
        printf("IMU sensor is not enabled\n");
        IsIMUEnabled = false;
    }
    return IsIMUEnabled;
}
//**************************************************************************
// Update all sensors
//**************************************************************************

void Sensors::update() {
    // Read raw measurements from the imu sensor
    updateIMU();
    // Scale raw measurements 
    ax_ /= G_SI;
    ay_ /= G_SI;
    az_ /= G_SI;
    // Apply calibration offset
    gx_ -= gyro_offset_[0];
    gy_ -= gyro_offset_[1];
    gz_ -= gyro_offset_[2];
}
//**************************************************************************
// Calibrate gyro sensor
//**************************************************************************

void Sensors::gyroCalibrate() {
    //--------------------------------------------------------------------------
    printf("Beginning Gyro calibration...\n");
    int maxCount = 500;
    for (int i = 0; i < maxCount; i++) {
        updateIMU();
        gyro_offset_[0] -= gx_;
        gyro_offset_[1] -= gy_;
        gyro_offset_[2] -= gz_;
        usleep(10000);
    }
    gyro_offset_[0] /= maxCount;
    gyro_offset_[1] /= maxCount;
    gyro_offset_[2] /= maxCount;

    printf("Gyro offsets are: %f %f %f\n", gyro_offset_[0], gyro_offset_[1], gyro_offset_[2]);
    storeInfo();
}
//**************************************************************************
// Update imu sensor and store the row measurements
//**************************************************************************

void Sensors::updateIMU() {
    // Read raw measurements from the imu sensor
    imu_->update();
    imu_->read_gyroscope(&gx_, &gy_, &gz_);
    imu_->read_accelerometer(&ax_, &ay_, &az_);
    imu_->read_magnetometer(&mx_, &my_, &mz_);
   // printf("%f %f %f",gx_,ax_,mx_);
    // Store row data
    storeRowData();
}
//**************************************************************************
// Store row measurements
//**************************************************************************

void Sensors::storeRowData() {
    // get current time stamp
    getTime();
    // Write data
    fprintf(row_data_file_, "%10ul %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f %5.5f\n",
            time_now_,
            gx_, gy_, gz_,
            ax_, ay_, az_,
            mx_, my_, mz_);
}
//**************************************************************************
// Get the current time 
//**************************************************************************

void Sensors::getTime() {
    // Timing data
    struct timeval tv;

    // Calculate delta time 
    gettimeofday(&tv, NULL);
    time_now_ = 1000000 * tv.tv_sec + tv.tv_usec;
}

//**************************************************************************
// Store sensors info
//**************************************************************************

void Sensors::storeInfo() {
    // get current time stamp
    getTime();
    // Write data
    fprintf(row_data_file_, "%10ul %5.5f %5.5f %5.5f\n",
            time_now_,
            gyro_offset_[0], gyro_offset_[1], gyro_offset_[2]);
}
