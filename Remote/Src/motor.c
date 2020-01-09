/**
 * motor.c
 * This header declares all motor related functions and variables.
 */

#include "motor.h"


/* Local defines*/

/* Global variables */
QueueHandle_t xMotorQueue;
TaskHandle_t xAnalogNotification;


/**
 * @brief   Starts PWM on target channel.
 * 
 * @param   ulChannel       PWM channel
 * @param   pxMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vStartMotor(const uint32_t ulChannel, TimerHandle_t *const pxMotorTimers)
{
    BaseType_t xAssert;
    
    /* Start software timer */
    xAssert = xTimerStart(pxMotorTimers[ulChannel], (TickType_t)0);
    configASSERT(xAssert);
    
    /* Enable PWM output on channel */
    PORTD->PCR[ulChannel] |= PORT_PCR_MUX(ALT4);
}


/**
 * @brief   Stops PWM on target channel.
 * 
 * @param   ulChannel       PWM channel
 * @param   pxMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vStopMotor(const uint32_t ulChannel, TimerHandle_t *const pxMotorTimers)
{
    BaseType_t xAssert;
    
    /* Stop software timer */
    xAssert = xTimerStop(pxMotorTimers[ulChannel], (TickType_t)0);
    configASSERT(xAssert);
        
    /* Disable PWM output on channel */
    PORTD->PCR[ulChannel] &= ~PORT_PCR_MUX(ALT4);
}


/**
 * @brief   Drives motors (water pumps) with PWM. PWM is triggered
 *          by boolean values and stopped by setting event bits.
 * 
 * @param   vMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vMotorTask(void *const pvMotorTimers)
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 10 / portTICK_PERIOD_MS;
    struct Motor_States *pxMotors;
    
    for (;;)
    {
        if (xQueueReceive(xMotorQueue, &pxMotors, (TickType_t)10))
        {
            for (uint32_t i = 0; i < MOTOR_COUNT; i++)
            {
                if (pxMotors->ucMotorState[i] == TRUE)
                {
                    vStartMotor(i, (TimerHandle_t *)pvMotorTimers);
                }
            }
        }
        
        uxBits = xEventGroupWaitBits(xMotorEventGroup, 0xF, pdTRUE, pdFALSE, xTicksToWait);
        for (uint32_t i = 0; i < MOTOR_COUNT; i++)
        {
            if (uxBits & (MASK(i) == MASK(i)))
            {
                pxMotors->ucMotorState[i] = FALSE;
                vStopMotor(i, (TimerHandle_t *)pvMotorTimers);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

