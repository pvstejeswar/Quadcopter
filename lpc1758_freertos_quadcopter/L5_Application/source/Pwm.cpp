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
    pwm1value = 6;
    pwm2value = 6;
    pwm3value = 6;
    pwm4value = 6;
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
    return true;
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
        pwm1value+=0.5;
        pwm2value+=0.5;
        pwm3value+=0.5;
        pwm4value+=0.5;

        if(pwm1value < 5 || pwm1value > 11)
            pwm1value = 6;

        if(pwm2value < 5 || pwm2value > 11)
            pwm2value = 6;

        if(pwm3value < 5 || pwm3value > 11)
            pwm3value = 6;

        if(pwm4value < 5 || pwm4value > 11)
            pwm4value = 6;

        pwm1.set(pwm1value);
        pwm2.set(pwm2value);
        pwm3.set(pwm3value);
        pwm4.set(pwm4value);
        LE.setAll(0x9);
        return;
    }

    if (cmd == motordown)
    {
        pwm1value-=0.5;
        pwm2value-=0.5;
        pwm3value-=0.5;
        pwm4value-=0.5;

        if(pwm1value < 5 || pwm1value > 11)
            pwm1value = 6;

        if(pwm2value < 5 || pwm2value > 11)
            pwm2value = 6;

        if(pwm3value < 5 || pwm3value > 11)
            pwm3value = 6;

        if(pwm4value < 5 || pwm4value > 11)
            pwm4value = 6;

        pwm1.set(pwm1value);
        pwm2.set(pwm2value);
        pwm3.set(pwm3value);
        pwm4.set(pwm4value);
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

