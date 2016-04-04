/*
 * Shared_variables.h
 *
 *  Created on: Mar 21, 2016
 *      Author: Tejeswar
 */

#ifndef L5_APPLICATION_SHARED_VARIABLES_H_
#define L5_APPLICATION_SHARED_VARIABLES_H_

#include "tasks.hpp"
#include <stdio.h>
#include "scheduler_task.hpp"
#include "utilities.h"
#include "uart3.hpp"
#include "lpc_pwm.hpp"

class BluetoothTask:public scheduler_task
{
        unsigned long taskRateHz;
        QueueHandle_t qh;
        Uart3& U3;

    public:
        BluetoothTask(unsigned long rateHz, uint8_t priority);
        bool run(void*p);
        bool init(void);

};

class PWMTask:public scheduler_task
{
        unsigned long taskRateHz;
        QueueHandle_t qh;
        PWM pwm1;
        PWM pwm2;
        PWM pwm3;
        PWM pwm4;
        float pwm1value;
        float pwm2value;
        float pwm3value;
        float pwm4value;

    public:
        PWMTask(unsigned long rateHz, uint8_t priority);
        bool run(void*p);
        void motorcmd(int cmd);

};

enum{
    bluettothCommand,
    motorinit,
    motorstop,
    motorhover,
    motorup,
    motordown,
};



#endif /* L5_APPLICATION_SHARED_VARIABLES_H_ */
