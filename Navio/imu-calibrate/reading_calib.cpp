/* 
 * File:   main.cpp
 * Author: Bara Emran
 *
 * Created on February 3, 2018, 11:58 AM
 */

#include <stdio.h>
#include <sys/time.h>

//******************************************************************************
// Main function
//******************************************************************************

int main(int argc, char *argv[]) {

    float ax_mid, ay_mid, az_mid;
    float ax_sen ,ay_sen, az_sen;
    char str1[5], str2[5];
   
    FILE* file = fopen("acc_calib.txt","r");

    fscanf(file,"%s %f %f %f\n", str1, &ax_mid, &ay_mid, &az_mid);
    fscanf(file,"%s %f %f %f\n", str2, &ax_sen ,&ay_sen, &az_sen);        
    
    printf("%5s values for x-axis: %+5.5f  z-axis: %+5.5f  z-axis: %+5.5f\n",
            str1,ax_mid,ay_mid,az_mid);
    printf("%5s values for x-axis: %+5.5f  z-axis: %+5.5f  z-axis: %+5.5f\n",
            str2,ax_sen,ay_sen,az_sen);

  return 0;

}
