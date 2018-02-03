/*
To achieve stable loop you need to run this application with a high priority
on a linux kernel with real-time patch. Raspbian distribution with real-time
kernel is available at emlid.com and priority can be set with chrt command:
chrt -f -p 99 PID
 */
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include "AHRS.hpp"
#include "IMU.hpp" 

unsigned long previoustime, currenttime;
float getdt(float Hz);

//******************************************************************************
// Print help message
//******************************************************************************

void print_help() {
    printf("Possible parameters:\nSensor selection: -i [sensor name]\n");
    printf("Sensors names: mpu is MPU9250, lsm is LSM9DS1\nFor help: -h\n");
    printf("If you want to visualize IMU data on another machine,\n");
    printf("add IP address and port number (by default 7000):\n");
    printf("-i [sensor name] ipaddress portnumber\n");

}

//******************************************************************************
// Main function
//******************************************************************************

int main(int argc, char *argv[]) {
    int parameter;
    char *sensor_name;
    bool is_network = false;
    
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
    IMU *imu = new IMU(sensor_name);
    if (imu->is == NULL) {
        printf("Wrong sensor name. Select: mpu or lsm\n");
        return EXIT_FAILURE;
    }

    // Setup IMU
    imu->imuSetup();

    // Setup AHRS
    AHRS ahrs;
    ahrs.setGyroOffset(imu->gyroOffset[0], imu->gyroOffset[1], imu->gyroOffset[2]);
    
    // create file to store data
    FILE * dataFile;
    dataFile = fopen ("data.txt","w");
    
    // Network data.
    int sockfd;
    struct sockaddr_in servaddr = {0};
    char sendline[80];

    // Network setup
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;

    if (argc == 5) {
        servaddr.sin_addr.s_addr = inet_addr(argv[3]);
        servaddr.sin_port = htons(atoi(argv[4]));
        is_network = true;
    }
    
    
    // Orientation data
    float roll, pitch, yaw;

    // Timing data
    float dt, maxdt;
    float mindt = 0.01;
    float dtsumm = 0;
    int isFirst = 1;
    dt = getdt(1300.0);
    while (1) {
        // Calculate delta time 
        dt = getdt(1300.0);

        // Discard the time of the first cycle
        if (!isFirst) {
            if (dt > maxdt) maxdt = dt;
            if (dt < mindt) mindt = dt;
        }
        isFirst = 0;
        
        // update imu data
        imu->update();

        // update AHRS
        ahrs.updateIMU(imu->ax, imu->ay, imu->az, imu->gx, imu->gy, imu->gz, dt);

        // Read Euler angles 
        ahrs.getEuler(&roll, &pitch, &yaw);

        fprintf (dataFile, "%10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f\n",
                  dt,
                  imu->gx,imu->gy,imu->gz,
                  imu->ax,imu->ay,imu->az,
                  imu->mx,imu->my,imu->mz,
                  roll, pitch, yaw,
                  ahrs.getW(), ahrs.getX(), ahrs.getY(), ahrs.getZ());
    
        //------------- Console and network output with a lowered rate ------------
        dtsumm += dt;
        if (dtsumm > 0.05) {
            // Console output
            printf("ROLL: %+05.2f PITCH: %+05.2f YAW: %+05.2f PERIOD %.4fs RATE"
                    " %dHz \n", roll, pitch, yaw * -1, dt, int(1 / dt));

            // Network output
            if (is_network){
                sprintf(sendline, "%10f %10f %10f %10f %dHz\n", ahrs.getW(), ahrs.getX(), ahrs.getY(), ahrs.getZ(), int(1 / dt));
                sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &servaddr, sizeof (servaddr));
            }
            dtsumm = 0;
        }
    }
    fclose (dataFile);
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
    if (dt < 1 / Hz){
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
