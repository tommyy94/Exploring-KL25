#include "gpio.h"


void GPIO_Init(void)
{
    /* Enable clock to PORTD */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    
    /* Select GPIO for pin */
    PORTD->PCR[SIGNAL_SHIFT] = PORT_PCR_MUX(ALT1);
    
    /* Set port to output */
    FGPIOD->PDDR |= MASK(SIGNAL_SHIFT);
}
