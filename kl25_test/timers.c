#include "timers.h"


/* Local defines*/


/* Global variables */
volatile uint32_t g_sTicks = 0; /* Store second ticks */


/* Configures SysTick to use 3 MHz clock */
void SysTick_Init(void)
{
    /* Set reload to trigger interrupt every 1 second */
    SysTick->LOAD = (SystemCoreClock / 16);

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


void SysTick_Handler(void)
{
    /* Count seconds since CPU boot */
    g_sTicks++;
}


void DelayUs(const uint32_t us)
{
    /**
     * Get delta of SysTick and compare to delay parameter
     * SysTick configured to use 3 MHz clock => multiply delay by 3 to achieve 1 MHz = 1 us
     * NOTE: SysTick is decrementing timer
     */
    static const uint8_t clockMultiplier = 3;
    uint32_t delta = SysTick->VAL;
    
    while ((delta - SysTick->VAL) < (clockMultiplier * us)) 
    {
        ; /*  Wait until time passed */
    }
}
