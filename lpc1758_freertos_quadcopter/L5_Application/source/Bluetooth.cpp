/*
 * Bluetooth.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Tejeswar
 */

#include "shared_variables.h"
#include "uart2.hpp"
#include "string.h"

BluetoothTask::BluetoothTask(unsigned long rateHz, uint8_t priority):
scheduler_task("Bluetooth", 3 * 512, priority),taskRateHz(rateHz),U2(Uart2::getInstance())
{
    qh = NULL;
    setRunDuration(taskRateHz);
}

bool BluetoothTask::init(void)
{
    U2 = Uart2::getInstance();
    U2.init(38400,10,10);
#if 0
    delay_ms(1000);
    U2.put("\r\n+STWMOD=0\r\n",portMAX_DELAY);
    U2.put("\r\n+STNA=SJ-QUAD\r\n",portMAX_DELAY);
    U2.put("\r\n+STAUTO=0\r\n",portMAX_DELAY); // 1: auto connect to last connected device
    U2.put("\r\n+STOAUT=1\r\n",portMAX_DELAY);
    U2.put("\r\n +STPIN=0000\r\n",portMAX_DELAY);
    delay_ms(2000); // This delay is required.
    U2.put("\r\n+INQ=1\r\n",portMAX_DELAY);
    delay_ms(2000);// This delay is required.
#endif
    return true;
}

bool BluetoothTask::run(void* p)
{
    qh = getSharedObject(bluetoothCommand);
    char command[20]= {0};
    U2.gets(command,20,1000);
    if(strlen(command) > 0)
    {
        printf("Command is :-- %s\n",command);
        if(!xQueueSend(qh, command, portMAX_DELAY)) {
            //printf("Failed Sending Command in 20ms");
        }
    }
    return true;
}




