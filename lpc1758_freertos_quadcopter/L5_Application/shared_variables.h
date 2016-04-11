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
#include <math.h>
#include "scheduler_task.hpp"
#include "utilities.h"
#include "uart3.hpp"
#include "lpc_pwm.hpp"


#define GYRO_CONST 131.0
#define ACC_CONST 16384.0   /*Don't remove .0*/

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
        int min, max, avg;
        double  acc_x, acc_y, acc_z;  //TODO: Need to change location of variables ????
        double  gyr_x, gyr_y, gyr_z;

    public:
        GyroTask(unsigned long rateHz, uint8_t priority);
        bool run(void*p);

};


class PWMTask:public scheduler_task
{
        unsigned long taskRateHz;
        QueueHandle_t blth,gyro;
        PWM pwm1, pwm2, pwm3, pwm4;

        float pid_p_gain_roll;
        float pid_i_gain_roll;
        float pid_d_gain_roll;

        float pid_p_gain_pitch;
        float pid_i_gain_pitch;
        float pid_d_gain_pitch;

        float pid_p_gain_yaw;
        float pid_i_gain_yaw;
        float pid_d_gain_yaw;

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
