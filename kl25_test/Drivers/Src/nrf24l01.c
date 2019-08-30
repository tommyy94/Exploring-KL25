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
 * @note    Because of full-duplex mode SPI receive buffer is polled while DMA
 *          is transferring through SPI transmit buffer.
 * 
 * @param   ucRegister      Register to read.
 * 
 * @return  value           Register value.
 */
uint8_t nRF24L01_ucReadRegister(const uint8_t ucRegister)
{
    uint8_t ucValue;
    
    /* Begin transfer */
    SPI1_vSetSlave(LOW);
    
    /* Status returned on first write */
    SPI1_vTransmitByte(R_REGISTER | ucRegister);
    (void)SPI1_ucReadPolling(); /* Discard status */
    
    /* Read register after second write */
    SPI1_vTransmitByte(R_REGISTER | ucRegister);
    ucValue = SPI1_ucReadPolling();
    
    /* End transfer */
    SPI1_vSetSlave(HIGH);    
    
    return (ucValue);
}


/**
 * @brief   Write nRF24L01 register.
 * 
 * @param   ucRegister      Register to write.
 * 
 * @param   ucValue         Value to write.
 * 
 * @return  None
 */
void nRF24L01_vWriteRegister(const uint8_t ucRegister, const uint8_t ucValue)
{    
    const uint8_t ucData[] = { W_REGISTER | ucRegister, ucValue, 0x00 }; /* Add null-terminator for strlen() */

    /* First transfer register, then value */
    SPI1_vTransmitDMA((const char *)ucData);
    
}
