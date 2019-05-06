#include "hw_timers.h"


/* Local defines*/
#define TPM0_CH0_PWM_PIN    (0UL)
#define TPM1_IC_PIN         (13UL)

/* Global variables */


/* TODO: Figure out correct/adjustable duty cycle and period */
/* Initialized to PWM mode. */
void TPM0_vInit(uint16_t usPeriod)
{
    /* Turn on clock gating for TPM0 and PORTD */
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
    
    /* Set clock source for TPM0 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /* Select pin multiplexer for TPM0 */
    PORTD->PCR[TPM0_CH0_PWM_PIN] |= PORT_PCR_MUX(ALT4);
    
    /* Load counter */
    TPM0->MOD |= usPeriod - 1;
    
    /* Continue in debug mode */
    TPM0->CONF = TPM_CONF_DBGMODE(1);
    
    /* Prescaler 2 */
    TPM0->SC = TPM_SC_CPWMS(1) | TPM_SC_PS(1);
    
    /* Set channel 0 to center-aligned PWM */
    TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSA(1);
    
    /* Set duty cycle */
    TPM0->CONTROLS[0].CnV = 4800;
    
    /* Start TPM0 */
    TPM0->SC |= TPM_SC_CMOD(1);
}


void TPM0_vStartPWM(uint8_t channel)
{
    PORTD->PCR[channel] |= PORT_PCR_MUX(ALT4);
}


void TPM0_vStopPWM(uint8_t channel)
{
    PORTD->PCR[channel] &= ~PORT_PCR_MUX(ALT4);
}


/* Initialized to Input Capture mode. */
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


void TPM1_IRQHandler(void)
{
    static uint32_t ulOverflows = 0;
    
    /* Check for counter overflows */
    if (TPM1->STATUS & TPM_STATUS_TOF_MASK)
    {
        ulOverflows++;
    }
    
    if (TPM1->STATUS & TPM_STATUS_CH1F_MASK)
    {
        /* Stop TPM1 */
        TPM1->SC &= ~TPM_SC_CMOD_MASK;
        
        /* TODO: Convert this capacitor value to humidity */
        ulHS1101_value = TPM1->CONTROLS[1].CnV;
        
        /* Reset counter */
        TPM1->CNT = 0;
        ulOverflows = 0;
        ulHS1101_flag = TRUE;
    }
    
    /* Reset all flags */
    TPM1->STATUS |= TPM_STATUS_TOF_MASK | TPM_STATUS_CH1F_MASK;
    TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF(1);
}


/* TODO: Add FreeRTOS software timers */
/* TODO: Disable TMP0 when not used */
void vMotorTask(void *const param)
{
    (void)param;
    
    for (;;)
    {
        TPM0_vStartPWM(TPM0_CH0_PWM_PIN);
        vTaskDelay(MSEC_TO_TICK(500));
        TPM0_vStopPWM(TPM0_CH0_PWM_PIN);
        vTaskDelay(MSEC_TO_TICK(500));
    }
}
