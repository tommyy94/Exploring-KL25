#include <includes.h>


int main(void)
{
    static TimerHandle_t xMotorTimers[MOTOR_COUNT];
    
    vSystemInit();
    vCreateQueues();
    vCreateEvents();
    vCreateTimers(xMotorTimers);
    vCreateTasks(xMotorTimers);
    vTaskStartScheduler();
    
    for (;;)
    {
        ; /* Should not get here! */
    }
}
