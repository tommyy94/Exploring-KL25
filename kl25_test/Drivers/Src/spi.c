/**
 * spi.c
 * Driver module for MKL25 SPI peripheral.
 */


#include "spi.h"


/* Local defines */
#define MISO            (1UL)
#define SCK             (2UL)
#define MOSI            (3UL)
#define SS              (4UL)


/* Function descriptions */

/**
 * @brief   Initialize SPI1 peripheral. Manual SS used for full-duplex mode.
 * 
 * @details Baud rate = 48 MHz/(3*2²) = 4 MHz = 250 ns/bit
 * 
 * @param   None
 * 
 * @return  None
 */
void SPI1_vInit(void)
{
    /* Enable clock to SPI1 */
    SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Disable SPI during configuration */
    SPI1->C1 &= ~SPI_C1_SPE_MASK;
    
    /* Set PTE2 as SPI1_SCK */
    PORTE->PCR[SCK] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[SCK] |= PORT_PCR_MUX(ALT2);
    
    /* Set PTE3 as SPI1_MOSI */
    PORTE->PCR[MOSI] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[MOSI] |= PORT_PCR_MUX(ALT5);
    
    /* Set PTE1 as SPI1_MISO */
    PORTE->PCR[MISO] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[MISO] |= PORT_PCR_MUX(ALT5);
    
    /* Set PTE4 as manual SS */
    PORTE->PCR[SS] = PORT_PCR_MUX(ALT1);
    FGPIOE->PDDR |= MASK(SS);
    FGPIOE->PDOR |= MASK(SS);
    
    /* Select master mode */
    SPI1->C1 = SPI_C1_MSTR_MASK;
    
    /**
     * Select active high clock
     * First edge sample
     */
    SPI1->C1 &= ~(SPI_C1_CPHA_MASK & SPI_C1_CPOL_MASK);
    
    /* Baudrate = Bus clock / ((SPPR + 1) * 2^^(SPR+1)) */
    SPI1->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(1);
    
    /* Enable SPI1 */
    SPI1->C1 |= SPI_C1_SPE_MASK;
}


/**
 * @brief   Read byte from SPI1 receive buffer.
 * 
 * @param   None
 * 
 * @return  SPI1->D     Received byte
 */
uint8_t SPI1_ucReadPolling(void)
{
    while (!(SPI1->S & SPI_S_SPRF(1)))
    {
        ; /* Wait until buffer full */
    }
    
    return SPI1->D;
}


/**
 * @brief   Transmit character over SPI by polling.
 * 
 * @param   None
 * 
 * @return  None
 */
void SPI1_vTransmitByte(const char ucByte)
{
    while (!(SPI1->S  & SPI_S_SPTEF_MASK))
    {
        ; /* Wait until TX buffer empty */
    }
    
    /* Send character */
    SPI1->D = ucByte;
}


/**
 * @brief   Transmit string over SPI by polling.
 * 
 * @param   pcData      String to send
 *             
 * @return  None
 */
void SPI1_vTransmitPolling(const  char *pcData)
{
    /* Find size of array */
    uint16_t dataLen = (uint16_t)strlen(pcData);
    
    /* Send the array of characters */
    for (uint16_t i = 0; i < dataLen; i++)
    {
        SPI1_vTransmitByte(pcData[i]);
    }
}


/**
 * @brief   Transmit string over SPI by DMA.
 * 
 * @param   pcData      String to send
 *             
 * @return  None
 */
void SPI1_vTransmitDMA(const  char *pcData)
{
    const uint8_t ucLength = strlen(pcData);

    /* Set transfer duration */
    TPM2_vLoadCounter(ucLength);

    /* Set source and destination addresses */
    DMA0_vInitTransaction((uint32_t *)pcData, (uint32_t *)&(SPI1->D), ucLength);
    
    /* Begin transfer */
    SPI1_vSetSlave(LOW);

    TPM2_vStart();
    
    /**
     * Datasheet recommends starting transfer by reading status register
     * and sending first byte by placing value to register
     */
    (void)SPI1->S;
    SPI1->D = pcData[0];

    /* Enable DMA transmitter */
    BME_OR8(&SPI1->C2, SPI_C2_TXDMAE(1));
    
    /* Send rest of the bytes */
    DMA0_vStart();
}


/**
 * @brief   Set SS line high/low.
 * 
 * @param   ulState     HIGH/LOW
 *             
 * @return  None
 */
void SPI1_vSetSlave(const uint32_t ulState)
{
    /* Figure out whether to set or clear bit */
    const uint32_t *pulReg = (uint32_t *)&FGPIOE->PCOR - ulState; /* Subtract 0 - 1 words from PCOR address => pulReg = FGPIOE->PSOR/PCOR */
    
    /* Perform bitwise operation */
    BME_OR32(&(*pulReg), MASK(SS));
}
