#include "timers.h"


/* Local defines*/
#define F_TPM_CLOCK (48000000UL)
#define F_TPM_OVFLW (100UL)


/* Local global variables */
volatile uint32_t g_sTicks = 0; /* Store second ticks */


void SysTick_Init(void)
{
    /* Set reload to trigger interrupt every 1 second */
    SysTick->LOAD = (48000000UL / 16);

    /* Set interrupt priority */
    NVIC_SetPriority(SysTick_IRQn, 4);

    /* Force load of reload value */
    SysTick->VAL = 0;

    /* Enable SysTick timer */
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void WDT_Init(void)
{
    /**  
     * 1 kHz clock
     * 1024 cycle timeout
     */
    SIM->COPC = SIM_COPC_COPT(3) & ~SIM_COPC_COPCLKS_MASK & ~SIM_COPC_COPW_MASK;
}


void Service_COP_WDT(void)
{
    SIM->SRVCOP = 0x55;
    SIM->SRVCOP = 0xAA;
}


void TPM0_Init(void)
{
    /* Turn on clock to TPM0 */
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);

    /* Set clock source for TPM0 */
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;

    /* Disable TPM0 */
    TPM0->SC = 0;

    /* Load the counter and mod, given prescaler of 32 */
    TPM0->MOD = (F_TPM_CLOCK / (F_TPM_OVFLW * 32)) - 1;

    /**
     * Enable interrupts
     * Divide by 32 prescaler
     */
    TPM0->SC = TPM_SC_TOIE(1) | TPM_SC_PS(5);
    
    /* Set NVIC for TPM0 ISR */
    NVIC_SetPriority(TPM0_IRQn, 3);
    NVIC_ClearPendingIRQ(TPM0_IRQn);
    NVIC_EnableIRQ(TPM0_IRQn);
    
    /* Enable counter */
    TPM0->SC |= TPM_SC_CMOD(1);
}


void TPM0_IRQHandler(void)
{
    /* Reset overflow flag */
    TPM0->SC |= TPM_SC_TOIE(1);
}


void SysTick_Handler(void)
{
    /* Count seconds since CPU boot */
    g_sTicks++;
}
