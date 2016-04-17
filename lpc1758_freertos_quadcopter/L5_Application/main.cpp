#include "shared_variables.h"
#include "tasks.hpp"
#include "scheduler_task.hpp"



int main(void)
{
    //scheduler_add_task(new PWMTask(100,PRIORITY_HIGH));
    //scheduler_add_task(new BluetoothTask(100,PRIORITY_HIGH));
    scheduler_add_task(new GyroTask(1,PRIORITY_HIGH));
    scheduler_start(); ///< This shouldn't return
    return -1;
}
