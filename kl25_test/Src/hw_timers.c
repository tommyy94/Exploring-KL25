#include "hw_timers.h"


/* Local defines*/
#define TPM1_IC_PIN (13UL)

/* Global variables */


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
