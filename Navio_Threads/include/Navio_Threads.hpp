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
#include "SamplingTime.h"    // sampling time

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
};

// ********************************************************** //
// Functions prototype:
void *conThread(void *data);
void *senThread(void *data);
void *tcpThread(void *data);
void ctrlCHandler(int signal);

#endif /* NAVIO_THREADS_HPP */

W