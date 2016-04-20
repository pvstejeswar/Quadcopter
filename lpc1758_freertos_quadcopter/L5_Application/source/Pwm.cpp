/*
 * Pwm.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Tejeswar
 */

#include "shared_variables.h"
#include "string.h"
#include "io.hpp"

PWMTask::PWMTask(unsigned long rateHz, uint8_t priority) :
scheduler_task("pwm", 3 * 512, priority), taskRateHz(rateHz), pwm1(
        PWM::pwm1, 50), pwm2(PWM::pwm2, 50), pwm3(PWM::pwm3, 50), pwm4(
                PWM::pwm4, 50)
{
    pwm_throttle = 7;
    pid_i_mem_roll = 0;
    pid_last_roll_d_error = 0;
    pid_i_mem_pitch = 0;
    pid_last_pitch_d_error = 0;
    pid_i_mem_yaw = 0;
    pid_last_yaw_d_error = 0;
    pid_error_temp = 0;

    pid_output_pitch = pid_output_roll = pid_output_yaw = 0;
    gyro_pitch_input = gyro_roll_input = gyro_yaw_input = 0;

    pid_pitch_setpoint = 0;
    pid_yaw_setpoint = 0;
    pid_roll_setpoint = 0;

    esc_1 = esc_2 = esc_3 = esc_4 = 0;

    pid_p_gain_roll = pid_p_gain_pitch = 0.09;
    pid_i_gain_roll = pid_i_gain_pitch = 0.0002;
    pid_d_gain_roll = pid_d_gain_pitch = 0.0001;

    pid_p_gain_yaw = 4.0;
    pid_i_gain_yaw = 0.02;
    pid_d_gain_yaw = 0.0;

    gyro = xQueueCreate(1, sizeof(Orientation));
    blth = xQueueCreate(1, 20);
    addSharedObject(IMUData,gyro);
    addSharedObject(bluetoothCommand,blth);
    setRunDuration(taskRateHz);
}

bool PWMTask::run(void *p)
{

#if 0
    char command[20] = { 0 };
    if (!xQueueReceive(blth, command, 10))
    {
        puts("Failed to receive bluetooth command in 20 ms");
    }
    else
    {
        printf("Received: %s\n", command);
        if (strcmp("init", command) == 0)
            motorcmd(motorinit);
        else if (strcmp("up", command) == 0)
            motorcmd(motorup);
        else if (strcmp("down", command) == 0)
            motorcmd(motordown);
        else if (strcmp("hover", command) == 0)
            motorcmd(motorhover);
        else if (strcmp("stop", command) == 0)
            motorcmd(motorstop);
    }
#endif
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

    calculate_pid();

    esc_1 = pwm_throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;
    esc_2 = pwm_throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;
    esc_3 = pwm_throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;
    esc_4 = pwm_throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;

    esc_1 = (esc_1 < 7)?7:(esc_1>11)?11:esc_1;
    esc_2 = (esc_2 < 7)?7:(esc_2>11)?11:esc_2;
    esc_3 = (esc_3 < 7)?7:(esc_3>11)?11:esc_3;
    esc_4 = (esc_4 < 7)?7:(esc_4>11)?11:esc_4;


    printf("\n------- ESC Values -------\nESC1: %f\nESC2: %f\nESC3: %f\nESC4: %f\n",
            esc_1,esc_2,esc_3,esc_4);


    pwm1.set(esc_1);
    pwm2.set(esc_2);
    pwm3.set(esc_3);
    pwm4.set(esc_4);

    return true;
}

void PWMTask::calculate_pid(){
    //Roll calculations
    pid_error_temp = (gyro_roll_input - pid_roll_setpoint)/5;
    pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
    pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
    pid_last_roll_d_error = pid_error_temp;

    //Pitch calculations
    pid_error_temp = (gyro_pitch_input - pid_pitch_setpoint)/5;
    pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
    pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
    pid_last_pitch_d_error = pid_error_temp;

    //Yaw calculations
    pid_error_temp = (gyro_yaw_input - pid_yaw_setpoint);
    pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
    pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
    pid_last_yaw_d_error = pid_error_temp;

    printf("\n\nGyro Angles ---> Roll %lf Pitch %lf\n", gyro_roll_input, gyro_pitch_input);
    printf("Pid Output roll %lf pitch %lf\n",pid_output_roll,pid_output_pitch);

}


void PWMTask::motorcmd(int cmd)
{
    if (cmd == motorinit)
    {
        pwm1.set(11);
        pwm2.set(11);
        pwm3.set(11);
        pwm4.set(11);
        delay_ms(1000);
        pwm1.set(5);
        pwm2.set(5);
        pwm3.set(5);
        pwm4.set(5);
        LE.setAll(0xF);
        return;
    }
    if (cmd == motorstop)
    {
        pwm1.set(5);
        pwm2.set(5);
        pwm3.set(5);
        pwm4.set(5);
        LE.setAll(0);
        return;
    }
    if (cmd == motorup)
    {
        pwm_throttle+=0.5;
        if(pwm_throttle < 5 || pwm_throttle > 11)
            pwm_throttle = 6;
        LE.setAll(0x9);
        return;
    }

    if (cmd == motordown)
    {
        pwm_throttle-=0.5;
        if(pwm_throttle < 5 || pwm_throttle > 11)
            pwm_throttle = 6;
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

