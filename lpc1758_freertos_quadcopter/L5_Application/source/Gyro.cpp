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
    avg = 7;
    min = 7;
    max = 7;
    acc_x = acc_y = acc_z = 0;
    gyr_x = gyr_y = gyr_z = 0;
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

    double roll_gyr  = gyr_x * 0.01;
    double pitch_gyr = gyr_y * 0.01;


    printf("\n\nx %lf y %lf z %lf", acc_x, acc_y, acc_z);
    printf("\nx %lf y %lf z %lf", gyr_x, gyr_y, gyr_z);

    printf("\n\nACCE --- Roll %lf Pitch %lf", roll_acc, pitch_acc);
    printf("\nGYRO --- Roll %lf Pitch %lf", roll_gyr, pitch_gyr);
    printf("\nValidate Who am i %d", IMU.validate());

    /*if (min > z)
            min = z;
    if (max < z)
            max = z;
    avg = (avg + z)/2;

    printf("\nY %d min %d max %d avg %d diff_avg %d diff %d\n", z, min, max, avg, z - avg, z - prev_z);
    printf("\nX - %d\n", IMU.GS_getX());
    printf("Y - %d\n", IMU.GS_getY());
    printf("Z - %d\n", IMU.GS_getZ());
    prev_z = z;*/
    //delay_ms(1000);
    return true;
}
