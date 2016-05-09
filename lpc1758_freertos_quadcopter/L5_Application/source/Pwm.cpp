/*
 * Pwm.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Tejeswar
 */

#include "shared_variables.h"
#include "string.h"
#include "io.hpp"
#include "time.h"
#include <math.h>

#define ESC_MAX 9.5
#define ESC_MIN 7.5
#define ESC_STOP 6


PWMTask::PWMTask(unsigned long rateHz, uint8_t priority) :
scheduler_task("pwm", 3 * 512, priority), taskRateHz(rateHz), pwm1(
        PWM::pwm1, 50), pwm2(PWM::pwm2, 50), pwm3(PWM::pwm3, 50), pwm4(
                PWM::pwm4, 50)
{
    pwm_throttle = ESC_MIN;
    pid_i_mem_roll = 0;
    pid_last_roll_d_error = 0;
    pid_i_mem_pitch = 0;
    pid_last_pitch_d_error = 0;
    pid_i_mem_yaw = 0;
    pid_last_yaw_d_error = 0;
    pid_error_temp = 0;
    armed = false;

    pid_output_pitch = pid_output_roll = pid_output_yaw = 0;
    gyro_pitch_input = gyro_roll_input = gyro_yaw_input = 0;

    pid_pitch_setpoint = 0;
    pid_yaw_setpoint = 0;
    pid_roll_setpoint = 0;

    esc_1 = esc_2 = esc_3 = esc_4 = 0;

    pid_p_gain_roll = pid_p_gain_pitch = 0.01;
    pid_i_gain_roll = pid_i_gain_pitch = 0.0001;
    pid_d_gain_roll = pid_d_gain_pitch = 0.0002;

    pid_p_gain_yaw = 4.0;
    pid_i_gain_yaw = 0.02;
    pid_d_gain_yaw = 0.2;

    gyro = xQueueCreate(5, sizeof(Orientation));
    blth = xQueueCreate(1, 20);
    addSharedObject(IMUData,gyro);
    addSharedObject(bluetoothCommand,blth);
    setRunDuration(taskRateHz);
}

bool PWMTask::run(void *p)
{

    char command[20] = { 0 };
    if (!xQueueReceive(blth, command, 10))
    {
        //printf("Failed to receive command in 20 ms");
    }
    else
    {
        printf("Received: %s\n", command);
        if(strcmp("start", command) == 0)
        {
            armed = true;
            motorcmd(motorinit);
        }
        else if (strcmp("stop", command) == 0)
        {
            armed = false;
            motorcmd(motorstop);
        }
        else if (strcmp("up", command) == 0)
        {
            motorcmd(motorup);
        }
        else if (strcmp("down", command) == 0)
        {
            motorcmd(motordown);
        }

    }

    Orientation ori;
    if (!xQueueReceive(gyro, &ori, 10))
    {
        puts("Failed to receive Gyro Data");
    }
    else
    {
        gyro_roll_input  = ori.roll;
        gyro_pitch_input = ori.pitch;
        gyro_yaw_input   = ori.yaw;
    }

    if(armed == true)
    {
        calculate_pid();

        esc_1 = pwm_throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;
        esc_2 = pwm_throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;
        esc_3 = pwm_throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;
        esc_4 = pwm_throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;

        esc_1 = floorf(esc_1 *100)/100;
        esc_2 = floorf(esc_2 *100)/100;
        esc_3 = floorf(esc_3 *100)/100;
        esc_4 = floorf(esc_4 *100)/100;

        esc_1 = (esc_1 < ESC_MIN)?ESC_MIN:(esc_1>ESC_MAX)?ESC_MAX:esc_1;
        esc_2 = (esc_2 < ESC_MIN)?ESC_MIN:(esc_2>ESC_MAX)?ESC_MAX:esc_2;
        esc_3 = (esc_3 < ESC_MIN)?ESC_MIN:(esc_3>ESC_MAX)?ESC_MAX:esc_3;
        esc_4 = (esc_4 < ESC_MIN)?ESC_MIN:(esc_4>ESC_MAX)?ESC_MAX:esc_4;


        printf("TIME IS %lu",time(NULL));

        printf("\n------- ESC Values -------\nESC1: %f\nESC2: %f\nESC3: %f\nESC4: %f\n",
                esc_1,esc_2,esc_3,esc_4);

        pwm1.set(esc_1);
        pwm2.set(esc_2);
        pwm3.set(esc_3);
        pwm4.set(esc_4);
    }
    return true;
}

void PWMTask::calculate_pid(){
    //Roll calculations
    pid_error_temp = (int)(gyro_roll_input - pid_roll_setpoint)/5;
    pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
    pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
    pid_last_roll_d_error = pid_error_temp;

    //Pitch calculations
    pid_error_temp = (int)(gyro_pitch_input - pid_pitch_setpoint)/5;
    pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
    pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
    pid_last_pitch_d_error = pid_error_temp;

    //Yaw calculations
    pid_error_temp = (int)(gyro_yaw_input - pid_yaw_setpoint)/5;
    pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
    pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
    pid_last_yaw_d_error = pid_error_temp;

    printf("\n\nGyro Angles ---> Roll %lf Pitch %lf\n", gyro_roll_input, gyro_pitch_input);
    printf("Pid Output roll %lf pitch %lf\n",pid_output_roll,pid_output_pitch);

    static int i=0;
    if(i == 5)
    {
        Uart2 &u2 = Uart2::getInstance();
        u2.printf("\nGyro Angles ---> Roll %lf Pitch %lf\n", gyro_roll_input, gyro_pitch_input);
        u2.printf("Pid Output roll %lf pitch %lf\n",pid_output_roll,pid_output_pitch);
        u2.printf("------- ESC Values -------\nESC1: %f\nESC2: %f\nESC3: %f\nESC4: %f\n",
                        esc_1,esc_2,esc_3,esc_4);
        i=0;
    }
    i++;
}


void PWMTask::motorcmd(int cmd)
{
    if (cmd == motorinit)
    {

        LE.setAll(0xF);
        return;
    }
    if (cmd == motorstop)
    {
        pwm1.set(ESC_STOP);
        pwm2.set(ESC_STOP);
        pwm3.set(ESC_STOP);
        pwm4.set(ESC_STOP);
        LE.setAll(0);
        return;
    }
    if (cmd == motorup)
    {
        pwm_throttle+=0.25;
        LE.setAll(0x9);
        return;
    }

    if (cmd == motordown)
    {
        pwm_throttle-=0.25;
        LE.setAll(0x6);
        return;
    }
    if (cmd == motorhover)
    {
        pwm1.set(8);
        pwm2.set(8);
        pwm3.set(8);
        pwm4.set(8);
        LE.setAll(0xA);
        return;
    }

}

