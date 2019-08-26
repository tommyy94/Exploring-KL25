/**
 * spi.c
 * Driver module for MKL25 SPI peripheral.
 */


#include "spi.h"


void SPI1_vInit(void)
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
    
    /* Set PTE1 as SPI1_MISO -- ALT5 */
    PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[1] |= PORT_PCR_MUX(ALT5);
    
    /* Set PTE4 as SPI1_PCS0 -- ALT2 */
    PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[4] |= PORT_PCR_MUX(ALT2);
    
    /**
     * Select master mode
     * Enable SS
     */
    SPI1->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
    
    /* Master mode-fault function enable */
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
    /* Disable DMA receiver, enable transmitter */
    BME_AND8(&SPI1->C2, ~(uint8_t )SPI_C2_RXDMAE(1));
    BME_OR8(&SPI1->C2, SPI_C2_TXDMAE(1));
    
    /**
     * Datasheet recommends starting transfer by reading status register
     * and sending first byte by placing value to register
     */
    (void)SPI1->S;
    SPI1->D = pcData[0];
    
    /* Send rest of the bytes */
    DMA0_vStart((uint32_t *)pcData);
}
