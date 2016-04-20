/*
 * Bluetooth.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Tejeswar
 */

#include "shared_variables.h"
#include "uart3.hpp"
#include "string.h"

BluetoothTask::BluetoothTask(unsigned long rateHz, uint8_t priority):
scheduler_task("Bluetooth", 3 * 512, priority),taskRateHz(rateHz),U3(Uart3::getInstance())
{
    qh = NULL;
    setRunDuration(taskRateHz);
}

bool BluetoothTask::init(void)
{
    U3 = Uart3::getInstance();
    U3.init(38400,10,10);
    delay_ms(1000);
    U3.put("\r\n+STWMOD=0\r\n",portMAX_DELAY);
    U3.put("\r\n+STNA=SJQUAD\r\n",portMAX_DELAY);
    U3.put("\r\n+STAUTO=0\r\n",portMAX_DELAY);
    U3.put("\r\n+STOAUT=1\r\n",portMAX_DELAY);
    U3.put("\r\n +STPIN=0000\r\n",portMAX_DELAY);
    delay_ms(2000); // This delay is required.
    U3.put("\r\n+INQ=1\r\n",portMAX_DELAY);
    delay_ms(2000);// This delay is required.
    return true;
}

bool BluetoothTask::run(void* p)
{
    qh = getSharedObject(bluetoothCommand);
    char command[20]= {0};
    U3.gets(command,20,1000);
    if(strlen(command) > 0)
    {
        printf("Command is :-- %s\n",command);
        if(!xQueueSend(qh, command, portMAX_DELAY)) {
            printf("Failed Sending Bluetooth Command in 20ms");
        }
    }
    return true;
}




