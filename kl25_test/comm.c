#include "comm.h"


/* Local defines */
#define SYS_CLOCK (48000000UL)
#define UART0_OVERSAMPLE_RATE (16)
#define TX_PIN 2


void UART0_Init(const uint32_t baudrate)
{
    uint16_t sbr;
    
    /* Enable clock gating for UART0 & PORTA */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    /* Disable transmitter & receiver */
    UART0->C2 &= ~UART0_C2_TE_MASK;
    
    /* Set UART0 clock to 48 MHz */
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    /* set pins to UART0 TX */
    PORTA->PCR[TX_PIN] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);
    
    /**
     * Set baudrate
     * Set oversampling ratio
     */
    sbr = (uint16_t)(SYS_CLOCK / (baudrate * UART0_OVERSAMPLE_RATE));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0->BDL = UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(UART0_OVERSAMPLE_RATE - 1);
    
    /* Select one stop bit */
    UART0->BDH |= UART0_BDH_SBNS(0);
    
    /**
     * Disable loopback mode
     * 8-bit mode
     * No parity bits
     */
    UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
    
    /**
     * Don't invert TX data
     * Enable interrupts
     */
    UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1) | UART0_C3_PEIE(1);
    
    /* Clear error flags */
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
    
    /* Send LSB first */
    UART0->S2 = UART0_S2_MSBF(0);
    
    /* Enable TX */
    UART0->C2 |= UART0_C2_TE(1);
}


void UART0_TransmitPolling(const char *data)
{
    /* Find size of array */
    uint16_t dataLen =  (uint16_t)(uint16_t)strlen(data);
    
    /* Send the array of characters */
    for (uint16_t i = 0; i < dataLen; i++)
    {
        while (!(UART0->S1 & UART0_S1_TDRE_MASK))
        {
            ; /* Wait until TX buffer empty */
        } 
        
        /* Send character */
        UART0->D = data[i];
    }
}
