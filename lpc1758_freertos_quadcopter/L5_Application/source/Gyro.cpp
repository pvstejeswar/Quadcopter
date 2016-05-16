/*
 * Gyro.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: Tejeswar
 */

#include "shared_variables.h"
#include "io.hpp"

GyroTask::GyroTask(unsigned long rateHz, uint8_t priority):
scheduler_task("pwm", 3 * 512, priority), taskRateHz(rateHz)
{
    qh = NULL;
    prevtimer = sys_get_uptime_ms();
    currtimer = sys_get_uptime_ms();
    acc_x = acc_y = acc_z = 0;
    gyr_x = gyr_y = gyr_z = 0;
    roll = pitch = yaw = 0;
    mag_x = mag_y = mag_z = 0;
    Filtered_pitch = Filtered_roll = 0;
    setRunDuration(taskRateHz);
}

bool GyroTask::run(void*p )
{
    /* Convert raw values to meter per second2 */
    acc_x = IMU.AS_getX() / ACC_CONST;
    acc_y = IMU.AS_getY() / ACC_CONST;
    acc_z = IMU.AS_getZ() / ACC_CONST;

    double roll_acc  = atan2(-acc_x, acc_z) * 180/M_PI;
    double pitch_acc = atan2(-acc_y, sqrt(acc_x * acc_x + acc_z * acc_z)) * 180/M_PI;

    /* TODO: Magnetometer value calculations....*/
    mag_x = IMU.M_getX() ;
    mag_y = IMU.M_getY() ;
    mag_z = IMU.M_getZ() ;

    currtimer = sys_get_uptime_ms();
    float diff = currtimer - prevtimer;

    /*Convert raw value to degree per second*/
    gyr_x = IMU.GS_getX()/ GYRO_CONST;
    gyr_y = IMU.GS_getY()/ GYRO_CONST;
    gyr_z = IMU.GS_getZ()/ GYRO_CONST;
    prevtimer = currtimer;

    gyr_x = gyr_x * diff/1000;
    gyr_y = gyr_y * diff/1000;

    double roll_gyr = roll  +  gyr_x;
    double pitch_gyr= pitch + gyr_y;

    roll  = GAINFACTOR * roll_gyr + (1- GAINFACTOR)* roll_acc;
    pitch = GAINFACTOR * pitch_gyr + (1- GAINFACTOR)* pitch_acc;

    roll = (roll > -90)?(roll <90?roll:90):(-90);
    pitch = (pitch > -90)?(pitch <90?pitch:90):(-90);

    if(pitch > 30)
        roll=0;

    qh = getSharedObject(IMUData);
    Orientation ori;
    if(Filtered_roll - roll < 15 && Filtered_pitch - pitch < 15)
    {
        Filtered_pitch = 0.8*(Filtered_pitch) + 0.2*(pitch +1.05);
        Filtered_roll = 0.8*(Filtered_roll) + 0.2*(roll - 8.10);
    }
    ori.pitch = Filtered_pitch;
    ori.roll = Filtered_roll;
    ori.yaw  = 0;//yaw;

    if(!xQueueSend(qh, &ori,10))
    {
        printf("--- Failed To Send Gyro Data ---");
    }

#if 0
    printf("\n-----------------------------------------------------------\n");

    printf("Acc Raw Values are\nx %lf y %lf z %lf\n", acc_x, acc_y, acc_z);
    printf("Gyr Raw Values are\nx %lf y %lf z %lf\n", gyr_x, gyr_y, gyr_z);

    printf("ACC ---> Roll %lf Pitch %lf\n", roll_acc, pitch_acc);
    printf("GYRO ---> Roll %lf Pitch %lf\n", roll_gyr, pitch_gyr);
    printf("Total Angle is ---> Roll %lf Pitch %lf\n", roll, pitch);

    printf("\n----------------------------------------------------------\n");
#endif
    return true;
}
