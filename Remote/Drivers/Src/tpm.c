/**
 * tpm.c
 * Driver module for MKL25 TPM peripheral.
 */

#include "tpm.h"

/* Local defines */
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
void TPM0_vInit(void)
{    
    /* Set clock source for TPM0 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /* Select pin multiplexer for TPM0 */
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
        PORTD->PCR[i] |= PORT_PCR_MUX(ALT4);
    }
    
    /* Load counter */
    TPM0->MOD |= 4800 - 1;
    
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
        TPM0->CONTROLS[i].CnV = 30000;
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
     * Enable timer overflow interrupts
     * Divide by 128 prescaler
     */
    TPM1->SC = TPM_SC_PS(7) | TPM_SC_TOIE(1);
    
    /* Set NVIC for TPM1 ISR */
    NVIC_SetPriority(TPM1_IRQn, 3);
    NVIC_ClearPendingIRQ(TPM1_IRQn);
    NVIC_EnableIRQ(TPM1_IRQn);
}


/**
 * @brief   Initialize TPM2.
 * 
 * @param   None
 * 
 * @return  None
 */
void TPM2_vInit(void)
{
    /* Set clock source for TPM2 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /**
     * Enable timer overflow interrupts
     * Divide by 2 prescaler => 24 MHz clock speed
     */
    TPM2->SC = TPM_SC_PS(1) | TPM_SC_TOIE(1);

    /* Clear Timer Overflow Flag */
    TPM2->STATUS |= 0xFFFFFFFF;
    
    /* Reset counters */
    TPM2->CNT = 0;

    /* Set NVIC for TPM2 ISR */
    NVIC_SetPriority(TPM2_IRQn, 3);
    NVIC_ClearPendingIRQ(TPM2_IRQn);
    NVIC_EnableIRQ(TPM2_IRQn);
}


/**
 * @brief   Wrapper function for loading TPM2 counter.
 * 
 * @detail  TPM2 is configured to 48 MHz/2 prescaler = 24 MHz clock speed.
 * 
 * @note    timePerBit = 250 ns
 *          => timePerByte = 2 탎
 *          timeBetweenBytes = 0.25 탎
 *          => timePerByte = 2.25 탎
 *          
 *          deliveryTime = timePerByte * numberOfBytes - timeBetweenBytes
 * 
 *          Note: 2 탎 overhead at beginning
 * 
 * @param   ulBytes     Number of bytes to send.
 * 
 * @return  None
 */
void TPM2_vLoadCounter(uint32_t ulBytes)
{
    TPM2->MOD = TIME_PER_BYTE * ulBytes - TIME_BETWEEN_BYTES; /* Last delay between bytes not needed */
}


/**
 * @brief   Wrapper function for starting TPM2 counter.
 * 
 * @param   None
 * 
 * @return  None
 */
void TPM2_vStart(void)
{
    BME_OR32(&TPM2->SC, TPM_SC_CMOD(1));
}


/**
 * @brief   Wrapper function for stopping TPM2 counter.
 * 
 * @param   None
 * 
 * @return  None
 */
void TPM2_vStop(void)
{
    BME_AND32(&TPM2->SC, ~TPM_SC_CMOD(1));
}
