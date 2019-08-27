/**
 * nrf24l01.c
 * Driver for NRF24l01.
 */

#include "nrf24l01.h"


/* Local defines */
#define CE              (1UL)       /* Chip Enable */
#define IRQ             (2UL)       /* Interrupt Request */

#define NOP             (0xFFUL)    /* No Operation to read STATUS register */
#define R_REGISTER      (0x0UL)     /* Read command and status registers */
#define W_REGISTER      (0x0UL)     /* Write command and status registers - power down/standby modes only */

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
    
    /* Enable NRF24L01 */
    FGPIOA->PDOR |= MASK(CE);
}


/**
 * @brief   Read nRF24L01 register value.
 * 
 * @param   ucRegister      Register to read.
 * 
 * @return  value           Register value.
 */
uint32_t nRF24L01_ulReadRegister(uint8_t ucRegister)
{
    uint32_t value;
    
    /* Begin transfer */
    SPI1_vSetSlave(LOW);
    
    /* Full-duplex needs to be receiving data when it's sending */
    SPI1_vTransmitByte(R_REGISTER + NOP);           /* Status returned on first write */
    SPI1_vTransmitByte(R_REGISTER + ucRegister);    /* Read register after second write */
    
    /* TODO: Add timeout/Implement DMA transfer/Implement interrupt on RX */
    value = SPI1_ucReadPolling();
    
    /* End transfer */
    SPI1_vSetSlave(HIGH);
    
    return (value);
}


void nRF24L01_vWriteRegister(uint8_t ucValue)
{
    
}
