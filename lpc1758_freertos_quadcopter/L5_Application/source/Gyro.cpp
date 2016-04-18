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

    acc_x = acc_y = acc_z = 0;
    gyr_x = gyr_y = gyr_z = 0;
    roll_gyr = pitch_gyr = 0;
    roll = yaw = pitch = 0;
}

bool GyroTask::run(void*p )
{
    /*int z = IMU.GS_getY();
    static int prev_z = 0;*/

    /* Convert raw values to meter per second2 */
    acc_x = IMU.AS_getX() / ACC_CONST;
    acc_y = IMU.AS_getY() / ACC_CONST;
    acc_z = IMU.AS_getZ() / ACC_CONST;

    /*Convert raw value to degree per second*/
    gyr_x = IMU.GS_getX() / GYRO_CONST;
    gyr_y = IMU.GS_getY() / GYRO_CONST;
    gyr_z = IMU.GS_getZ() / GYRO_CONST;


    /* TODO: Magnetometer value calculations....*/



    /*Calculate angle here...   X axis --> Roll, Y axis --> pitch, Z axis -->yaw */

    //double roll_acc  = atan2(acc_y, acc_z) * 180/M_PI;
    //double pitch_acc = atan2(-acc_x, sqrt(acc_y * acc_y + acc_z * acc_z)) * 180/M_PI;

    /* Y axis --> Roll, X axis --> pitch, ??? axis -->yaw */
    double roll_acc  = atan2(-acc_x, acc_z) * 180/M_PI;
    double pitch_acc = atan2(-acc_y, sqrt(acc_x * acc_x + acc_z * acc_z)) * 180/M_PI;

    roll_gyr  = roll + gyr_x * 0.01;
    pitch_gyr = pitch + gyr_y * 0.01;

    roll = (roll_gyr * GAIN) + (roll_acc * (1 - GAIN));
    pitch = (pitch_gyr * GAIN) + (pitch_acc * (1 - GAIN));

    printf("\n---------------------------------------------------");
    printf("\nACC --> x %lf y %lf z %lf", acc_x, acc_y, acc_z);
    printf("\nGYRO -->  x %lf y %lf z %lf", gyr_x, gyr_y, gyr_z);

    printf("\n\nACCE ---> Roll %lf Pitch %lf", roll_acc, pitch_acc);
    printf("\nGYRO ---> Roll %lf Pitch %lf", roll_gyr, pitch_gyr);
    printf("\nTotal ---> Roll %lf Pitch %lf", roll, pitch);
    printf("\n---------------------------------------------------\n");

    return true;
}
