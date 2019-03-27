#include "comm.h"


/* Local defines */
#define UART0_OVERSAMPLE_RATE   (16UL)
#define UART0_MAX_MSG_LEN       (128UL)


__STATIC_INLINE void UART0_TransmitByte(const char byte);
__STATIC_INLINE void UART0_TransmitPolling(const char *data);


/* Function descriptions */
void UART0_Init(const uint32_t baudrate)
{
    register uint16_t sbr;
    
    /* Enable clock gating for UART0 & PORTE */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Disable transmitter & receiver */
    UART0->C2 &= ~UART0_C2_TE_MASK;
    
    /* Set UART0 clock to 48 MHz */
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    /* set pins to UART0 TX */
    PORTE->PCR[UART0_TX_PIN] = PORT_PCR_MUX(ALT4);
    
    /**
     * Set baudrate
     * Set oversampling ratio
     */
    sbr = (uint16_t)(SystemCoreClock / (baudrate * UART0_OVERSAMPLE_RATE));
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
    
    /* Don't invert TX data */
    UART0->C3 = UART0_C3_TXINV(0);
    
    /* Clear error flags */
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
    
    /* Send LSB first */
    UART0->S2 = UART0_S2_MSBF(0);
    
    /* Enable TX */
    UART0->C2 |= UART0_C2_TE(1);
}


__STATIC_INLINE void UART0_TransmitByte(const char byte)
{
    while (!(UART0->S1 & UART0_S1_TDRE_MASK))
    {
        ; /* Wait until TX buffer empty */
    } 
        
    /* Send character */
    UART0->D = byte;
}


__STATIC_INLINE void UART0_TransmitPolling(const char *data)
{
    /* Find size of array */
    uint16_t dataLen = (uint16_t)strlen(data);
    
    /* Send the array of characters */
    for (uint16_t i = 0; i < dataLen; i++)
    {
        UART0_TransmitByte(data[i]);
    }
}


void UART0_printf(const char *p_fmt, ...)
{
    char string[UART0_MAX_MSG_LEN];
    va_list xArgp;

    /* Initialize variable arguments */
    va_start(xArgp, p_fmt);

    /* Format string */
    if (vsprintf(string, p_fmt, xArgp) > 0)
    {
        UART0_TransmitPolling(string);
    }

    va_end(xArgp);
}


void SPI1_Init(void)
{
    /* Enable clock to SPI1 */
    SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Disable SPI during configuration */
    SPI1->C1 &= ~SPI_C1_SPE_MASK;
    
    /* Set PTE2 as SPI1_SCK -- ALT2 */
    PORTE->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[2] |= PORT_PCR_MUX(ALT2);
    
    /* Set PTE3 as SPI1_MOSI -- ALT5 */
    PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[3] |= PORT_PCR_MUX(ALT5);
    
    /* Set PTE1 as SPI1_Miso -- ALT5 */
    PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[1] |= PORT_PCR_MUX(ALT5);
    
    /* Set PTE4 as SPI1_pcs0 -- ALT2 */
    PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[4] |= PORT_PCR_MUX(ALT2);
    
    /**
     * Select master mode
     * Enable SS
     */
    SPI1->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
    SPI1->C2 = SPI_C2_MODFEN_MASK;
    
    /**
     * Select active high clock
     * First edge sample
     */
    SPI1->C1 &= ~SPI_C1_CPHA_MASK;
    SPI1->C1 &= ~SPI_C1_CPOL_MASK;
    
    /* Baudrate = Bus clock / ((SPPR + 1) * 2^^(SPR+1)) */
    SPI1->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(1);
    
    /* Enable SPI1 */
    SPI1->C1 |= SPI_C1_SPE_MASK;
}


void SPI1_TransmitPolling(const  char *data)
{
    /* Find size of array */
    uint16_t dataLen = (uint16_t)strlen(data);
    
    /* Send the array of characters */
    for (uint16_t i = 0; i < dataLen; i++)
    {
        while (!(SPI1->S  & SPI_S_SPTEF_MASK))
        {
            ; /* Wait until TX buffer empty */
        }
    
        /* Send character */
        SPI1->D = data[i];
    }
}
