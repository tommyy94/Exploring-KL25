/**
 * nrf24l01.c
 * Driver for NRF24l01.
 */

#include "nrf24l01.h"


/* Local defines */
#define CE              (1UL)       /* Chip Enable */
#define IRQ             (2UL)       /* Interrupt Request */

#define NOP             (0xFFUL)    /* No Operation to read STATUS register */
#define R_REGISTER      (0x00UL)    /* Read command and status registers */
#define W_REGISTER      (0x20UL)    /* Write command and status registers - power down/standby modes only */

/* Function descriptions */

/**
 * @brief   Initialize NRF24L01 and masters peripherals.
 * 
 * @param   None
 * 
 * @return  None
 */
void nRF24L01_vInit(void)
{
    /* Enable PORTA clock gating */
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
    
    /* Select GPIO  */
    PORTA->PCR[CE] = PORT_PCR_MUX(ALT1);
    
    /* Set output */
    FGPIOA->PDDR |= MASK(CE);
    
    /* Enable nRF24L01 */
    FGPIOA->PDOR |= MASK(CE);
}


/**
 * @brief   Read nRF24L01 register value.
 * 
 * @param   ucRegister      Register to read.
 * 
 * @return  value           Register value.
 */
uint8_t nRF24L01_ucReadRegister(const uint8_t ucRegister)
{
    uint8_t ucValue;
    
    /* Full-duplex needs to be receiving data when it's sending */
    SPI1_vTransmitByte(NOP);                        /* Status returned on first write */
    (void)SPI1_ucReadPolling();                     /* TODO: Add timeout/Implement DMA transfer/Implement interrupt on RX */
    
    SPI1_vTransmitByte(R_REGISTER | ucRegister);    /* Read register after second write */
    ucValue = SPI1_ucReadPolling();                 /* TODO: Add timeout/Implement DMA transfer/Implement interrupt on RX */
    
    return (ucValue);
}


/**
 * @brief   Write nRF24L01 register.
 * 
 * @param   ucRegister      Register to write.
 * @param   ucValue         Value to write.
 * 
 * @return  None
 */
void nRF24L01_vWriteRegister(const uint8_t ucRegister, const uint8_t ucValue)
{    
    const uint8_t ucData[] = { ucRegister, ucValue, 0x00 }; /* Add null-terminator for strlen() */
    
    /* First transfer register, then value */
    SPI1_vTransmitDMA((const char *)ucData);
}
