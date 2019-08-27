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


uint32_t nRF24L01_ulReadRegister(uint8_t ucRegister)
{
    uint32_t byte;
    
    FGPIOE->PDOR &= ~MASK(4);
    
    for (uint8_t i = 0; i < 32; i++)
    {
        SPI1_vTransmitByte(R_REGISTER + ucRegister);
    }
    
    byte = SPI1_ucReadPolling();
    FGPIOE->PDOR |= MASK(4);
    return (byte);
}
