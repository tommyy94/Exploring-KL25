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
 * @param   ucChannel       PWM channel
 * @param   pxMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vStartMotor(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers)
{
    BaseType_t xAssert;
    
    /* Start software timer */
    xAssert = xTimerStart(pxMotorTimers[ucChannel], (TickType_t)0);
    configASSERT(xAssert);
    
    /* Enable PWM output on channel */
    PORTD->PCR[ucChannel] |= PORT_PCR_MUX(ALT4);
}


/**
 * @brief   Stops PWM on target channel.
 * 
 * @param   ucChannel       PWM channel
 * @param   pxMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vStopMotor(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers)
{
    BaseType_t xAssert;
    
    /* Stop software timer */
    xAssert = xTimerStop(pxMotorTimers[ucChannel], (TickType_t)0);
    configASSERT(xAssert);
        
    /* Disable PWM output on channel */
    PORTD->PCR[ucChannel] &= ~PORT_PCR_MUX(ALT4);
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
            for (uint8_t i = 0; i < MOTOR_COUNT; i++)
            {
                if (pxMotors->ucMotorState[i] == TRUE)
                {
                    vStartMotor(i, (TimerHandle_t *)pvMotorTimers);
                }
            }
        }
        
        uxBits = xEventGroupWaitBits(xMotorEventGroup, 0xF, pdTRUE, pdFALSE, xTicksToWait);
        for (uint8_t i = 0; i < MOTOR_COUNT; i++)
        {
            if (uxBits & (MASK(i) == MASK(i)))
            {
                pxMotors->ucMotorState[i] = FALSE;
                vStopMotor(i, (TimerHandle_t *)pvMotorTimers);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(100));
    }
}


/**
 * @brief   TPM1 IRQ Handler used to capture CMP0 output.
 * 
 * @param   None
 * 
 * @return  None
 * @todo    Convert capacitor value (HS1101_ulValue) to humidity.
 */
void TPM1_IRQHandler(void)
{ 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulOverflows = 0;
    uint32_t HS1101_ulValue = 0;
    BaseType_t xAssert;
    
    if (BME_UBFX32(&TPM1->STATUS, TPM_STATUS_TOF_SHIFT, 1))
    {
        ulOverflows++;
    }
    
    if (BME_UBFX32(&TPM1->STATUS, TPM_STATUS_CH1F_SHIFT, 1))
    {
        /* Overflows should not happen */
        configASSERT(ulOverflows == 0);
        
        /* Stop TPM1 */
        BME_AND32(&TPM1->SC, ~TPM_SC_CMOD(1));
        
        /* Read humidity */
        HS1101_ulValue = TPM1->CONTROLS[1].CnV;
        
        /* Conversion should have been in progress */
        configASSERT(xAnalogNotification != NULL);
        
        /* Notify task */
        xAssert = xTaskNotifyFromISR(xAnalogNotification, HS1101_ulValue, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        configASSERT(xAssert == pdPASS);
        
        /* No conversion in progress, so no tasks to notify */
        xAnalogNotification = NULL;
        
        /* Reset counters */
        TPM1->CNT = 0;
        ulOverflows = 0;
    }
    
    /* Reset all flags */
    BME_OR32(&TPM1->STATUS, TPM_STATUS_TOF(1) | TPM_STATUS_CH1F(1));
    BME_OR32(&TPM1->CONTROLS[1].CnSC, TPM_CnSC_CHF(1));
    
    /* Force context switch if xHigherPriorityTaskWoken is set to pdTRUE */
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
