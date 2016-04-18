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
    qh = xQueueCreate(1, 20);
    addSharedObject(gyroData,qh);
    setRunDuration(taskRateHz);
    prevtimer = sys_get_uptime_ms();
    currtimer = sys_get_uptime_ms();
    acc_x = acc_y = acc_z = 0;
    gyr_x = gyr_y = gyr_z = 0;
    gyr_x1 = gyr_x2 = gyr_x3 = 0;
    gyr_y1 = gyr_y2 = gyr_y3 = 0;
    roll_gyr = pitch_gyr = 0;
    roll = pitch = yaw = 0;
    mag_x = mag_y = mag_z = 0;
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
    mag_x = IMU.M_getX() / GYRO_CONST;
    mag_y = IMU.M_getY() / GYRO_CONST;
    mag_z = IMU.M_getZ() / GYRO_CONST;
    
    currtimer = sys_get_uptime_ms();
    float diff = currtimer - prevtimer;
    
    /*Convert raw value to degree per second*/
    gyr_x = IMU.GS_getX()/ GYRO_CONST;
    gyr_y = IMU.GS_getY()/ GYRO_CONST;
    gyr_z = IMU.GS_getZ()/ GYRO_CONST;
    prevtimer = currtimer;

    gyr_x = gyr_x * diff/1000;
    gyr_y = gyr_y * diff/1000;

    double roll_gyr = roll  +  (gyr_x1 + 1/6* (gyr_x3 + 2*gyr_x2 + 2*gyr_x1 + gyr_x));
    double pitch_gyr= pitch + (gyr_y1 + 1/6* (gyr_y3 + 2*gyr_y2 + 2*gyr_y1 + gyr_y));

    gyr_x3 = gyr_x2;
    gyr_x2 = gyr_x1;
    gyr_x1 = gyr_x;

    gyr_y3 = gyr_y2;
    gyr_y2 = gyr_y1;
    gyr_y1 = gyr_y;

    roll  = GAINFACTOR * roll_gyr + (1- GAINFACTOR)* roll_acc;
    pitch = GAINFACTOR * pitch_gyr + (1- GAINFACTOR)* pitch_acc;

    printf("\n-----------------------------------------------------------\n");

    printf("Acc Raw Values are\nx %lf y %lf z %lf\n", acc_x, acc_y, acc_z);
    printf("Gyr Raw Values are\nx %lf y %lf z %lf\n", gyr_x, gyr_y, gyr_z);

    printf("ACC ---> Roll %lf Pitch %lf\n", roll_acc, pitch_acc);
    printf("GYRO ---> Roll %lf Pitch %lf\n", roll_gyr, pitch_gyr);
    printf("Total Angle is ---> Roll %lf Pitch %lf\n", roll, pitch);

    printf("\n----------------------------------------------------------\n");

    return true;
}
