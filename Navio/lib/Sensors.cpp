/*
 * File:   Sensors.cpp
 * Author: Bara Emran
 * 
 * Created on February 3, 2018, 11:13 AM
 */

#include "Sensors.h"
//**************************************************************************
// Create sensor object and initiize default values
//**************************************************************************

Sensors::Sensors() {
    // Initialize default values
    imu_ = NULL;
    IsIMUEnabled = false;
    IsIMUCalibrated = false;
    gx_off_ = 0.0;
    gy_off_ = 0.0;
    gz_off_ = 0.0;
    ax_off_ = 0.0;
    ay_off_ = 0.0;
    az_off_ = 0.0;
    mx_off_ = 0.0;
    my_off_ = 0.0;
    mz_off_ = 0.0;
    ax_sen_ = 1.0;
    ay_sen_ = 1.0;
    az_sen_ = 1.0;
    mx_sen_ = 1.0;
    my_sen_ = 1.0;
    mz_sen_ = 1.0;

    // Create a file to store the row data
    row_data_file_ = fopen("row_data.txt", "w");
    printf("Start storing the data in the file \"row_data.txt\"\n");

    getTime(); // Initialize time variables the current time
    fprintf(row_data_file_, "Row data started in the following order:\n"
            "time, gx, gy, gz, ax, ay, az, mx, my, mz, temp, pres;\n");

    notes_file_ = fopen("info.txt","w");
    printf("Start storing the notes in the file \"info.txt\"\n");
}
//**************************************************************************
// Delete sensor object
//**************************************************************************

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
    // initialize chip
    imu_->initialize();

    // Check if imu sensor is enabled
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
// Create barometer sensor
//**************************************************************************

bool Sensors::createBarometer() {
    // Create and initialize barometer sensor
    printf("Create barometer sensor\n");
    barometer_->initialize();

    return true;
}
//**************************************************************************
// Update all sensors
//**************************************************************************

void Sensors::updateAll() {
    // Read measurements from the imu sensor
    updateIMU();

    // Read measurements from the barometer sensor
    updateBarometer();

    // Store the row data
    storeData();
}

//**************************************************************************
// Calibrate gyro sensor
//**************************************************************************

void Sensors::gyroCalibrate() {
    //--------------------------------------------------------------------------
    printf("Beginning Gyro calibration...\n");
    float maxCount = 500.0;
    float offset[3] ={0.0 ,0.0 ,0.0};    // use temp values to not change the gyro_offset variables
    for (int i = 0; i < int(maxCount); i++) {
        updateIMU();
        offset[0] = gx_;
        offset[1] = gy_;
        offset[2] = gz_;
        usleep(10000);
    }
    gx_off_ = offset[0] / maxCount;
    gy_off_ = offset[1] / maxCount;
    gz_off_ = offset[2] / maxCount;

    printf("Gyro offsets are: %f %f %f\n", gx_off_, gy_off_, gz_off_);
    storeInfo();
}
//******************************************************************************
// Read acceleromter calibration value from a specific file
//******************************************************************************

bool Sensors::accReadCalibration(char *file_name){

    char str1[8], str2[8];

    if( access( file_name, F_OK ) != -1 ) {
        // file exists
        printf ("Oppening file %s\n",file_name);

        FILE* file = fopen(file_name,"r");


        fscanf(file,"%s %f %f %f\n", str1, &ax_off_ , &ay_off_, &az_off_);
        fscanf(file,"%s %f %f %f\n", str2, &ax_sen_ , &ay_sen_, &az_sen_);

        printf("%5s values for x-axis: %+10.5f y-axis: %+10.5f z-axis: %+10.5f\n",
                str1, ax_off_, ay_off_, az_off_);
        printf("%5s values for x-axis: %+10.5f y-axis: %+10.5f z-axis: %+10.5f\n",
                str2, ax_sen_, ay_sen_, az_sen_);

        fscanf(file,"%s %f %f %f\n", str1, &mx_off_ , &my_off_, &mz_off_);
        fscanf(file,"%s %f %f %f\n", str2, &mx_sen_ , &my_sen_, &mz_sen_);

        printf("%5s values for x-axis: %+10.5f y-axis: %+10.5f z-axis: %+10.5f\n",
                str1, mx_off_, my_off_, mz_off_);
        printf("%5s values for x-axis: %+10.5f y-axis: %+10.5f z-axis: %+10.5f\n",
                str2, mx_sen_, my_sen_, mz_sen_);

        fclose(file);

	return 1;
    } else {
        // file doesn't exist
        printf ("Can't find the file %s\n",file_name);
        return 0;
    }
}
//**************************************************************************
// Read imu sensor
//**************************************************************************

void Sensors::updateIMU() {
    // Read raw measurements from the imu sensor
    imu_->update();
    imu_->read_gyroscope(&gx_, &gy_, &gz_);
    imu_->read_accelerometer(&ax_, &ay_, &az_);
    imu_->read_magnetometer(&mx_, &my_, &mz_);

    // rotate axis
    float tempax = ax_;
    ax_ = -ay_;
    ay_ = -tempax;
    float tempgx = gx_;
    gx_ = -gy_;
    gy_ = -tempgx;
    float tempmx = mx_;
    mx_ = -my_;
    my_ = -tempmx;

    // Scale raw measurements
    ax_ /= G_SI;
    ay_ /= G_SI;
    az_ /= G_SI;

    // Apply calibration offset
    gx_ = gx_ - gx_off_;
    gy_ = gy_ - gy_off_;
    gz_ = gz_ - gz_off_;

    // Apply calibration on accelerometer
    ax_ = (ax_ - ax_off_)/ax_sen_;
    ay_ = (ay_ - ay_off_)/ay_sen_;
    az_ = (az_ - az_off_)/az_sen_;

   // Apply calibration on accelerometer
    mx_ = (mx_ - mx_off_)/mx_sen_;
    my_ = (my_ - my_off_)/my_sen_;
    mz_ = (mz_ - mz_off_)/mz_sen_;

   // Store data_
   storeData();
}
//**************************************************************************
// Read Barometer sensor
//**************************************************************************

void Sensors::updateBarometer() {
    // Up raw measurements from the barometer sensor
    barometer_->update();
    // Calculate temperature and pressure values
    temp_ = barometer_->getTemperature();
    pres_ = barometer_->getPressure();
}
//**************************************************************************
// Store row measurements
//**************************************************************************

void Sensors::storeData() {
    // get current time stamp
    getTime();
    // Write data
    fprintf(row_data_file_, "%10ul, "
            "%+10.5f, %+10.5f, %+10.5f, "
            "%+10.5f, %+10.5f, %+10.5f, "
            "%+10.5f, %+10.5f, %+10.5f, "
            "%+10.5f, %+10.5f;\n",
            time_now_,
            gx_, gy_, gz_,
            ax_, ay_, az_,
            mx_, my_, mz_,
            temp_, pres_);
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
    fprintf(notes_file_, "storing information at time %10ul\n",
            time_now_);
    fprintf(notes_file_, "gyr_off %+10.5f, %+10.5f, %+10.5f;\n",
            gx_off_, gy_off_, gz_off_);
    fprintf(notes_file_, "acc_off %+10.5f, %+10.5f, %+10.5f;\n",
            ax_off_, ay_off_, az_off_);
    fprintf(notes_file_, "acc_sen %+10.5f, %+10.5f, %+10.5f;\n",
            ax_sen_, ay_sen_, az_sen_);
    fprintf(notes_file_, "mag_off %+10.5f, %+10.5f, %+10.5f;\n",
            mx_off_, my_off_, mz_off_);
    fprintf(notes_file_, "mag_sen %+10.5f, %+10.5f, %+10.5f;\n",
            mx_sen_, my_sen_, mz_sen_);
}

//**************************************************************************
// Display measurements
//**************************************************************************

void Sensors::displayData() {
    // get current time stamp
    getTime();
    // Write data
    printf("time: %10ul, "
           "gx=%10.5f, gy=%10.5f, gz=%10.5f, "
           "ax=%10.5f, ay=%10.5f, az=%10.5f, "
           "mx=%10.5f, my=%10.5f, mz=%10.5f, "
           "temperature=%10.5f, pressure=%10.5f;\n",
            time_now_,
            gx_, gy_, gz_,
            ax_, ay_, az_,
            mx_, my_, mz_,
            temp_, pres_);
}
