/**
 * tpm.c
 * Driver module for MKL25 TPM peripheral.
 */

#include "tpm.h"


#define TPM0_CH0_PWM_PIN    (0UL)
#define TPM1_IC_PIN         (13UL)

/* Function descriptions */

/**
 * @brief   Initialize TPM0 to PWM.
 * 
 * @param   usPeriod    PWM period
 * 
 * @return  None
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
