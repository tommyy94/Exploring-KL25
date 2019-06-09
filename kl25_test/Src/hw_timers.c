#include "hw_timers.h"


/* Local defines*/
#define TPM0_CH0_PWM_PIN    (0UL)
#define TPM1_IC_PIN         (13UL)

/* Global variables */
QueueHandle_t xMotorQueue;
TaskHandle_t xAnalogNotification;


/**
 * @brief   Initialize TPM0 to PWM.
 * 
 * @param   usPeriod    PWM period
 * 
 * @return  None
 * @todo    Figure out correct/adjustable duty cycle and period.
 */
void TPM0_vInit(const uint16_t usPeriod)
{
    /* Turn on clock gating for TPM0 and PORTD */
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
    
    /* Set clock source for TPM0 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /* Select pin multiplexer for TPM0 */
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        PORTD->PCR[i] |= PORT_PCR_MUX(ALT4);
    }
    
    /* Load counter */
    TPM0->MOD |= usPeriod - 1;
    
    /* Continue in debug mode */
    TPM0->CONF = TPM_CONF_DBGMODE(1);
    
    /* Prescaler 2 */
    TPM0->SC = TPM_SC_CPWMS(1) | TPM_SC_PS(1);
    
    /* Set PWM channels to center-aligned PWM */
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        TPM0->CONTROLS[i].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSA(1);
    }
    
    /* Set duty cycle */
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        TPM0->CONTROLS[i].CnV = 4800;
    }
    
    /* Start TPM0 */
    TPM0->SC |= TPM_SC_CMOD(1);
}


/**
 * @brief   Starts PWM on target channel.
 * 
 * @param   ucChannel       PWM channel
 * @param   pxMotorTimers   Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void TPM0_vStartPWM(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers)
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
void TPM0_vStopPWM(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers)
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
                    TPM0_vStartPWM(i, (TimerHandle_t *)pvMotorTimers);
                }
            }
        }
        
        uxBits = xEventGroupWaitBits(xMotorEventGroup, 0xF, pdTRUE, pdFALSE, xTicksToWait);
        for (uint8_t i = 0; i < MOTOR_COUNT; i++)
        {
            if (uxBits & (MASK(i) == MASK(i)))
            {
                pxMotors->ucMotorState[i] = FALSE;
                TPM0_vStopPWM(i, (TimerHandle_t *)pvMotorTimers);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(100));
    }
}


/**
 * @brief   Initialize TPM1 to Input Capture mode.
 * 
 * @param   None
 * 
 * @return  None
 */
void TPM1_vInit(void)
{
    /* Turn on clock gating for TPM1 and PORTA */
    SIM->SCGC6 |= SIM_SCGC6_TPM1(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
    
    /* Set clock source for TPM1 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /* Select pin multiplexer for TPM1 */
    PORTA->PCR[TPM1_IC_PIN] |= PORT_PCR_MUX(ALT3);
    
    /* Load the counter and mod */
    TPM1->MOD = 0xFFFF;

    /**
     * Input capture (Channel 1) with interrupt
     * Rising edge trigger
     */
    TPM1->CONTROLS[0].CnSC = TPM_CnSC_ELSB(1);
    TPM1->CONTROLS[1].CnSC = TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1);
    
    /**
     * Enable interrupts
     * Divide by 128 prescaler
     */
    TPM1->SC = TPM_SC_PS(7) | TPM_SC_TOIE(1);
    
    /* Set NVIC for TPM1 ISR */
    NVIC_SetPriority(TPM1_IRQn, 3);
    NVIC_ClearPendingIRQ(TPM1_IRQn);
    NVIC_EnableIRQ(TPM1_IRQn);
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
