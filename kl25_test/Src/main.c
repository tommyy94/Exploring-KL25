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
    TaskHandle_t xHandle;
    BaseType_t xAssert;
    static TimerHandle_t xMotorTimers[MOTOR_COUNT];
    
    /* Startup task creates other tasks */
    xAssert = xTaskCreate(vStartupTask, (const char *)"Startup", STARTUPTASKSIZE / sizeof(portSTACK_TYPE), xMotorTimers, STARTUPTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    /* Start multitasking */
    vTaskStartScheduler();
    
    for (;;)
    {
        ; /* Should not get here! */
    }
}
