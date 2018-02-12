/*
 * Done by: Bara Emran
 * Date: Aug 24, 2017
 * Purpose: Program that read IMU data from Navio board and apply AHRS to the 
 * measured signals to get Euler angles 
 */

#include "main.hpp"

//================================ Main loop ===================================
int main(int argc, char *argv[]) {
    structData data;
    // Timing data
    float dt, dtsumm = 0;
    //------------------------------ IMU setup ---------------------------------
    if (imuSetup("mpu") == NULL) {
        return -1;
    }
    
    SamplingTime ts(1000);  // create sampling time object
    while (1) {
        //----------------------- Calculate delta time -------------------------
        dt = ts.tsCalculat();
        //--------------------------- Read Sensor ------------------------------
        getIMU(&data,dt);
        //------------- Console and network output with a lowered rate ---------
        dtsumm += dt;
        if (dtsumm > 0.05) {
            dtsumm = 0;
            // Console output
            printf("ROLL: %+05.2f PITCH: %+05.2f YAW: %+05.2f PERIOD %.4fs RATE"
                    " %dHz \n", data.imu.r, data.imu.p, data.imu.w * -1, dt, 
                    int(1 / dt));
        }
    }
    return 0;
}

//============================= Initial setup =================================

InertialSensor* imuSetup(char *sensor_name) {
    
    //---------------------------- Create IMU ----------------------------------
    if (!strcmp(sensor_name, "mpu")) {
        printf("Selected: MPU9250\n");
        imu = new MPU9250();
    } else if(!strcmp(sensor_name, "lsm"))  {
        printf("Selected: LSM9DS1\n");
        imu = new LSM9DS1();
    } else {
        return NULL;
    }
    
    //------------------------ MPU initialization ------------------------------
    imu->initialize();

    //-------------------------- Calibrate gyro --------------------------------
    printf("Beginning Gyro calibration...\n");
    float gx, gy, gz;
    float offset[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < 100; i++) {
        imu->update();
        imu->read_gyroscope(&gx, &gy, &gz);
        offset[0] += -gx;
        offset[1] += -gy;
        offset[2] += -gz;
        usleep(10000);
    }
    offset[0] /= 100.0;
    offset[1] /= 100.0;
    offset[2] /= 100.0;
    
    //-------------------- Set & display offset result -------------------------
    printf("Offsets are: %f %f %f\n", offset[0], offset[1], offset[2]);
    ahrs.setGyroOffset(offset[0], offset[1], offset[2]);
    
    return imu;
}

//========================= Read IMU and update AHRS ===========================

void getIMU(structData* data, float dt) {
    
    //-------- Read raw measurements from the MPU and update AHRS --------------
    imu->update();
    imu->read_accelerometer(&data->imu.ax, &data->imu.ay, &data->imu.az);
    imu->read_gyroscope(&data->imu.gx, &data->imu.gy, &data->imu.gz);
    imu->read_magnetometer(&data->imu.mx, &data->imu.my, &data->imu.mz);

    // Scale Accelerometer measurement by dividing by 9.81
    data->imu.ax /= G_SI;
    data->imu.ay /= G_SI;
    data->imu.az /= G_SI;
    
    // Accelerometer + Gyro
    ahrs.updateIMU(data->imu.ax, data->imu.ay, data->imu.az,data->imu.gx,
    data->imu.gy, data->imu.gz, dt);
    
    // Accelerometer + Gyro + Magnetometer    
    //ahrs.update(data->imu.ax, data->imu.ay, data->imu.az,data->imu.gx, 
    //data->imu.gy, data->imu.gz,  data->imu.mx, data->imu.my, -data->imu.mz, dt);

    //------------------------ Read Euler angles ------------------------------
    ahrs.getEuler(&data->imu.r, &data->imu.p, &data->imu.w);
}
