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
    pwm_throttle = 6;
    pid_i_mem_roll = 0;
    pid_last_roll_d_error = 0;
    pid_i_mem_pitch = 0;
    pid_last_pitch_d_error = 0;
    pid_i_mem_yaw = 0;
    pid_last_yaw_d_error = 0;
    pid_pitch_setpoint = 0;
    pid_yaw_setpoint = 0;
    pid_roll_setpoint = 0;
    esc_1 = esc_2 = esc_3 = esc_4 = 0;

    pid_p_gain_roll = pid_p_gain_pitch = 1.4;
    pid_i_gain_roll = pid_i_gain_pitch = 0.05;
    pid_d_gain_roll = pid_d_gain_pitch =15;

    pid_p_gain_yaw = 4.0;
    pid_i_gain_yaw = 0.02;
    pid_d_gain_yaw = 0.0;


}

bool PWMTask::run(void *p)
{
    char command[20] = { 0 };
    blth = getSharedObject(bluetoothCommand);

    if (!xQueueReceive(blth, command, portMAX_DELAY))
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

    /*TODO : Receive GYRO Data */

    calculate_pid();

    esc_1 = pwm_throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;
    esc_2 = pwm_throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;
    esc_3 = pwm_throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;
    esc_4 = pwm_throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;

    pwm1.set(esc_1);
    pwm2.set(esc_2);
    pwm3.set(esc_3);
    pwm4.set(esc_4);

    return true;
}

void PWMTask::calculate_pid(){
    //Roll calculations
    pid_error_temp = gyro_roll_input - pid_roll_setpoint;
    pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
    pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
    pid_last_roll_d_error = pid_error_temp;

    //Pitch calculations
    pid_error_temp = gyro_pitch_input - pid_pitch_setpoint;
    pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
    pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
    pid_last_pitch_d_error = pid_error_temp;

    //Yaw calculations
    pid_error_temp = gyro_yaw_input - pid_yaw_setpoint;
    pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
    pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
    pid_last_yaw_d_error = pid_error_temp;
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

