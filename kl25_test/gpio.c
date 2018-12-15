#include "gpio.h"


void GPIO_Init(void)
{
    /* Enable clock to PORTA */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    
    /* Select GPIO for pin */
    PORTD->PCR[SIGNAL_SHIFT] = PORT_PCR_MUX(1);
    
    /* Set port to output */
    FGPIOD->PDDR |= MASK(SIGNAL_SHIFT);
}
