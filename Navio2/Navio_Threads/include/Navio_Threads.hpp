/* 
 * File:   Navio_Threads.hpp
 * Author: Bara Emran
 * Created on August 25, 2017, 10:18 AM
 */
#ifndef NAVIO_THREADS_HPP
#define NAVIO_THREADS_HPP

// ********************************************************** //
// Header files:
#include <pthread.h>    // create thread
#include <signal.h>     // signal ctrl+c
#include <stdio.h>      // printf
#include <unistd.h>     // usleep
#include "../../Navio/PWM.h"    // PWM
#include "../../Navio/Util.h"    // PWM
#define _MOTOR1 0
#define _MOTOR2 1
#define _MOTOR3 2
#define _MOTOR4 3
#define _MinSpeed 1.0 // ms
#define _MaxSpeed 2.0 // ms

// ********************************************************** //
// Global variables:
bool _CloseRequested = false;
pthread_t _Thread_Con;
pthread_t _Thread_Sen;
pthread_t _Thread_Tcp;

// ********************************************************** //
// Define structures:
struct dataStruct {
	char buff[100];
        float PWMval[4];
        PWM *pwm;
};

// ********************************************************** //
// Functions prototype:
void *conThread(void *data);
void *senThread(void *data);
void *tcpThread(void *data);
void ctrlCHandler(int signal);

int initPWM(PWM *pwm);
void setPWM(float PWMval[], float pwm1, float pwm2, float pwm3, float pwm4);
void setAllPWM(float PWMval[], float val);
void sendPWM(PWM* pwm, float PWMval[]);

#endif /* NAVIO_THREADS_HPP */