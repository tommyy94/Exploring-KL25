#include <includes.h>


/**
 * @brief   FreeRTOS software timer callback.
 * 
 * @param   None
 * 
 * @return  None
 */
int main(void)
{
    static TimerHandle_t xMotorTimers[MOTOR_COUNT];
    
    vSystemInit();
    vCreateQueues();
    vCreateSemaphores();
    vCreateEvents();
    vCreateTimers(xMotorTimers);
    vCreateTasks(xMotorTimers);
    vTaskStartScheduler();
    
    for (;;)
    {
        ; /* Should not get here! */
    }
}
