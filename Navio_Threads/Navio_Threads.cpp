/* 
 * File:   Navio_Threads.cpp
 * Author: Bara Emran
 * Created on August 25, 2017, 10:16 AM
 */

#include "Navio_Threads.hpp"
#include "SamplingTime.h"    // sampling time

#include <stdio.h>
//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <string.h>         // strlen
//#include <arpa/inet.h>      //inet_pton
#define PORT 8080

int main(int argc, char** argv) {
    // Welcome msg
    printf("Start Program\n"); 
    signal(SIGINT, ctrlCHandler);
    
    // Define main variables
    struct dataStruct data;

    // Start threads
    pthread_create(&_Thread_Con, NULL, conThread, (void *) &data);
    pthread_create(&_Thread_Sen, NULL, senThread, (void *) &data);
    pthread_create(&_Thread_Tcp, NULL, tcpThread, (void *) &data);

    // Main loop to read encoders value
    while (!_CloseRequested)
    {
            printf("main\n");
            sleep(1);
    }

    // Exit procedure
    pthread_cancel(_Thread_Con);
    pthread_cancel(_Thread_Sen);
    pthread_cancel(_Thread_Tcp);
    printf("Close program\n");        
    return 0;
}



// ********************************************************** //
// Detect ctrl+c
void ctrlCHandler(int signal)
{
	_CloseRequested = true;
	printf("Ctrl+c have been detected\n");
}

// ********************************************************** //
// Control thread
void *conThread(void *data)
{
	printf("Start control thread\n");

	struct dataStruct *my_data;
	my_data = (struct dataStruct *) data;
        SamplingTime st(400);
        float dt, dtsumm = 0;
        // Initialize PWM
	setAllPWM(my_data->PWMval, _MinSpeed);
	my_data->pwm = new PWM;

	// Initialize PWM
	if (!initPWM(my_data->pwm))
	{
		printf("Can not initialize PWM. Exit control thread\n");
		pthread_exit(NULL);
	}
        
	// Main loop
	while (!_CloseRequested)
	{   
            dt = st.tsCalculat();
            sendPWM(my_data->pwm, my_data->PWMval);
            dtsumm += dt;
            if (dtsumm > 1){
                dtsumm = 0;
                printf("control thread with %d Hz\n", int(1/dt));
            }
	}
	// Exit procedure
	printf("Exit control thread\n");
	pthread_exit(NULL);
}

// ********************************************************** //
// Sensor thread
void *senThread(void *data)
{
	printf("Start sensor thread\n");

	struct dataStruct *my_data;
	my_data = (struct dataStruct *) data;
        SamplingTime  st(1000);
        float dt, dtsumm = 0;
	// Main loop
	while (!_CloseRequested)
	{   
            dt = st.tsCalculat();
            dtsumm += dt;
            if (dtsumm > 1){
                dtsumm = 0;
                printf("sensor thread with %d Hz\n", int(1/dt));
            }
	}
	// Exit procedure
	printf("Exit sensor thread\n");
	pthread_exit(NULL);
}

// ********************************************************** //
// TCP thread
void *tcpThread(void *data)
{
	printf("Start tcp thread\n");

	struct dataStruct *my_data;
	my_data = (struct dataStruct *) data;
        SamplingTime  st(200);
        float dt, dtsumm = 0;
//         // Network data
//      
//        struct sockaddr_in address;
//        int sockfd = 0, valread;
//        struct sockaddr_in serv_addr;
//        char buffer[1024] = {0};
//        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//        {
//            printf("\n Socket creation error \n");
//            //return -1;
//        }
//  
//        memset(&serv_addr, '0', sizeof(serv_addr));
//        serv_addr.sin_family = AF_INET;
//        serv_addr.sin_port = htons(PORT);
//      
//        // Convert IPv4 and IPv6 addresses from text to binary form
//        inet_pton(AF_INET, "192.1681.1.100", &serv_addr.sin_addr);
//        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//        {
//            printf("\nConnection Failed \n");
//           // return -1;
//        }
//        
    
	// Main loop
	while (!_CloseRequested)
	{   
            dt = st.tsCalculat();
            dtsumm += dt;
            if (dtsumm > 1){
                dtsumm = 0;
                printf("tcp thread with %d Hz\n", int(1/dt));
                //sprintf(buffer, "%dHz\n", int(1 / dt));
                //sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &serv_addr, sizeof (serv_addr));    
            }
	}
	// Exit procedure
	printf("Exit tcp thread\n");
	pthread_exit(NULL);
}


int initPWM(PWM *pwm)
{
	if (!pwm->init(_MOTOR1) && !pwm->init(_MOTOR2) && !pwm->init(_MOTOR3) && !pwm->init(_MOTOR4))
	{
		fprintf(stderr, "Output Enable not set. Are you root?\n");
		return 0;
	}

	pwm->enable(_MOTOR1);
	pwm->enable(_MOTOR1);
	pwm->enable(_MOTOR1);
	pwm->enable(_MOTOR1);

	pwm->set_period(_MOTOR1, 400);
	pwm->set_period(_MOTOR1, 400);
	pwm->set_period(_MOTOR1, 400);
	pwm->set_period(_MOTOR1, 400);

	return 1;
}

// ********************************************************** //
void setPWM(float PWMval[], float pwm1, float pwm2, float pwm3, float pwm4)
{
	PWMval[0] = pwm1;
	PWMval[1] = pwm2;
	PWMval[2] = pwm3;
	PWMval[3] = pwm4;
}
// ********************************************************** //
void setAllPWM(float PWMval[], float val)
{
	setPWM(PWMval, val, val, val, val);
}
// ********************************************************** //
void sendPWM(PWM* pwm, float PWMval[])
{
	pwm->set_duty_cycle(_MOTOR1, PWMval[0]);
	pwm->set_duty_cycle(_MOTOR2, PWMval[1]);
	pwm->set_duty_cycle(_MOTOR3, PWMval[2]);
	pwm->set_duty_cycle(_MOTOR4, PWMval[3]);
}

