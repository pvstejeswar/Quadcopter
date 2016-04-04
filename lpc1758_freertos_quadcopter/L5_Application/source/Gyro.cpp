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
}

bool GyroTask::run(void*p )
{
    printf("AS_X----------: %d\n", IMU.AS_getX());
    printf("AS_Y----------: %d\n", IMU.AS_getY());
    printf("AS_Z----------: %d\n", IMU.AS_getZ());
    printf("\n\n\n");
    return true;
}


