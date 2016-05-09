#include "shared_variables.h"
#include "tasks.hpp"
#include "scheduler_task.hpp"

void esc_initialize()
{
    PWM pwm1(PWM::pwm1, 50);
    PWM pwm2(PWM::pwm2, 50);
    PWM pwm3(PWM::pwm3, 50);
    PWM pwm4(PWM::pwm4, 50);

    pwm1.set(6);
    pwm2.set(6);
    pwm3.set(6);
    pwm4.set(6);

    delay_ms(1000);
}



int main(void)
{
    esc_initialize();
    scheduler_add_task(new PWMTask(10,PRIORITY_HIGH));
    scheduler_add_task(new BluetoothTask(100,PRIORITY_MEDIUM));
    scheduler_add_task(new GyroTask(10,PRIORITY_MEDIUM));
    scheduler_start(); ///< This shouldn't return

    return -1;
}
