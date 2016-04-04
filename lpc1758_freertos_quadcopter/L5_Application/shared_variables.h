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

class GyroTask:public scheduler_task
{
        unsigned long taskRateHz;
        QueueHandle_t qh;

    public:
        GyroTask(unsigned long rateHz, uint8_t priority);
        bool run(void*p);

};


class PWMTask:public scheduler_task
{
        unsigned long taskRateHz;
        QueueHandle_t blth,gyro;
        PWM pwm1, pwm2, pwm3, pwm4;

        static float pid_p_gain_roll = 1.4;
        static float pid_i_gain_roll = 0.05;
        static float pid_d_gain_roll = 15;

        static float pid_p_gain_pitch = pid_p_gain_roll;
        static float pid_i_gain_pitch = pid_i_gain_roll;
        static float pid_d_gain_pitch = pid_d_gain_roll;

        float pid_p_gain_yaw = 4.0;
        float pid_i_gain_yaw = 0.02;
        float pid_d_gain_yaw = 0.0;

        float pid_i_mem_roll, pid_roll_setpoint, gyro_roll_input, pid_output_roll, pid_last_roll_d_error;
        float pid_i_mem_pitch, pid_pitch_setpoint, gyro_pitch_input, pid_output_pitch, pid_last_pitch_d_error;
        float pid_i_mem_yaw, pid_yaw_setpoint, gyro_yaw_input, pid_output_yaw, pid_last_yaw_d_error;
        float pid_error_temp;
        float pwm_throttle;
        float esc_1,esc_2,esc_3,esc_4;

    public:
        PWMTask(unsigned long rateHz, uint8_t priority);
        bool run(void*p);
        void motorcmd(int cmd);
        void calculate_pid();

};

enum{
    bluetoothCommand,
    gyroData,
    motorinit,
    motorstop,
    motorhover,
    motorup,
    motordown,
};


#endif /* L5_APPLICATION_SHARED_VARIABLES_H_ */
