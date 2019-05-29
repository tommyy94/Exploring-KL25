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
    static TimerHandle_t xTimeoutTimers[TIMEOUT_TIMER_COUNT];
    
    vSystemInit(xTimeoutTimers);
    vCreateQueues();
    vCreateSemaphores();
    vCreateEvents();
    vCreateMotorTimers(xMotorTimers);
    vCreateTimeoutTimers(xTimeoutTimers);
    vCreateTasks(xMotorTimers, xTimeoutTimers);
    vTaskStartScheduler();
    
    for (;;)
    {
        ; /* Should not get here! */
    }
}
