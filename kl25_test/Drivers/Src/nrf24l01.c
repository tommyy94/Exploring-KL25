/**
 * nrf24l01.c
 * Driver for NRF24l01.
 */

#include "nrf24l01.h"


/* Local defines */
#define CE                          (1UL)       /* Chip Enable */
#define IRQ                         (2UL)       /* Interrupt Request */

#define RXTX_ADDR_LEN               (5UL)

/* Commands */
#define NOP                         (0xFFUL)    /* No Operation to read STATUS register */
#define R_REGISTER                  (0x00UL)    /* Read command and status registers */
#define W_REGISTER                  (0x20UL)    /* Write command and status registers - power down/standby modes only */

/* Registers */
#define CONFIG                      (0x00UL)    /* Configuration Register */
#define EN_AA                       (0x01UL)    /* Enable Auto Acknowledgement */
#define EN_RXADDR                   (0x02UL)    /* Enabled RX Addresses */
#define SETUP_RETR                  (0x04UL)    /* Setup of Automatic Retransmission */
#define RF_CH                       (0x05UL)    /* RF Channel */
#define RF_SETUP                    (0x06UL)    /* RF Setup Register */
#define RX_ADDR_P0                  (0x0AUL)    /* Receive address data pipe 0 */
#define RX_ADDR_P1                  (0x0BUL)    /* Receive address data pipe 1 */
#define RX_ADDR_P2                  (0x0CUL)    /* Receive address data pipe 2 */
#define RX_ADDR_P3                  (0x0DUL)    /* Receive address data pipe 3 */
#define RX_ADDR_P4                  (0x0EUL)    /* Receive address data pipe 4 */
#define RX_ADDR_P5                  (0x0FUL)    /* Receive address data pipe 5 */
#define TX_ADDR                     (0x10UL)    /* Transmit address */
#define RX_PW_P0                    (0x11UL)    /* RX Payload Width Pipe 0 */
#define RX_PW_P1                    (0x12UL)    /* RX Payload Width Pipe 1 */
#define RX_PW_P2                    (0x13UL)    /* RX Payload Width Pipe 2 */
#define RX_PW_P3                    (0x14UL)    /* RX Payload Width Pipe 3 */
#define RX_PW_P4                    (0x15UL)    /* RX Payload Width Pipe 4 */
#define RX_PW_P5                    (0x16UL)    /* RX Payload Width Pipe 5 */

/* Register bits */
#define EN_AA_ENAA_P5(x)            (((uint8_t)(((uint8_t)(x)) << 5)) & 0xFFUL)
#define EN_AA_ENAA_P4(x)            (((uint8_t)(((uint8_t)(x)) << 4)) & 0xFFUL)
#define EN_AA_ENAA_P3(x)            (((uint8_t)(((uint8_t)(x)) << 3)) & 0xFFUL)
#define EN_AA_ENAA_P2(x)            (((uint8_t)(((uint8_t)(x)) << 2)) & 0xFFUL)
#define EN_AA_ENAA_P1(x)            (((uint8_t)(((uint8_t)(x)) << 1)) & 0xFFUL)
#define EN_AA_ENAA_P0(x)            (((uint8_t)(((uint8_t)(x)) << 0)) & 0xFFUL)

#define EN_RXADDR_ERX_P5(x)         (((uint8_t)(((uint8_t)(x)) << 5)) & 0xFFUL)
#define EN_RXADDR_ERX_P4(x)         (((uint8_t)(((uint8_t)(x)) << 4)) & 0xFFUL)
#define EN_RXADDR_ERX_P3(x)         (((uint8_t)(((uint8_t)(x)) << 3)) & 0xFFUL)
#define EN_RXADDR_ERX_P2(x)         (((uint8_t)(((uint8_t)(x)) << 2)) & 0xFFUL)
#define EN_RXADDR_ERX_P1(x)         (((uint8_t)(((uint8_t)(x)) << 1)) & 0xFFUL)
#define EN_RXADDR_ERX_P0(x)         (((uint8_t)(((uint8_t)(x)) << 0)) & 0xFFUL)

#define SETUP_RETR_ARD(x)           (((uint8_t)(((uint8_t)(x)) << 4)) & 0xFFUL)
#define SETUP_RETR_ARC(x)           (((uint8_t)(((uint8_t)(x)) << 0)) & 0xFFUL)

#define CONFIG_MASK_RX_DR(x)        (((uint8_t)(((uint8_t)(x)) << 6)) & 0xFFUL)
#define CONFIG_MASK_TS_DS(x)        (((uint8_t)(((uint8_t)(x)) << 5)) & 0xFFUL)
#define CONFIG_MASK_MAX_RT(x)       (((uint8_t)(((uint8_t)(x)) << 4)) & 0xFFUL)
#define CONFIG_MASK_EN_CRC(x)       (((uint8_t)(((uint8_t)(x)) << 3)) & 0xFFUL)
#define CONFIG_MASK_CRC0(x)         (((uint8_t)(((uint8_t)(x)) << 2)) & 0xFFUL)
#define CONFIG_MASK_PWR_UP(x)       (((uint8_t)(((uint8_t)(x)) << 1)) & 0xFFUL)
#define CONFIG_MASK_PRIM_RX(x)      (((uint8_t)(((uint8_t)(x)) << 0)) & 0xFFUL)

#define RF_SETUP_CONT_WAVE(x)       (((uint8_t)(((uint8_t)(x)) << 7)) & 0xFFUL)
#define RF_SETUP_RF_DR_LOW(x)       (((uint8_t)(((uint8_t)(x)) << 5)) & 0xFFUL)
#define RF_SETUP_PLL_LOCK(x)        (((uint8_t)(((uint8_t)(x)) << 4)) & 0xFFUL)
#define RF_SETUP_RF_DR_HIGH(x)      (((uint8_t)(((uint8_t)(x)) << 3)) & 0xFFUL)
#define RF_SETUP_RF_PWR(x)          (((uint8_t)(((uint8_t)(x)) << 1)) & 0xFFUL)



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
    PORTA->PCR[CE] = PORT_PCR_MUX(ALT1);
    FGPIOA->PDDR |= MASK(CE);
    
    /* Enable nRF24L01 */
    FGPIOA->PDOR |= MASK(CE);

    nRF24L01_vWriteRegister(RX_PW_P0, 22);
    nRF24L01_vWriteRegister(RF_CH, 50);

    nRF24L01_vWriteRegister(EN_AA, EN_AA_ENAA_P0(1));
    nRF24L01_vWriteRegister(SETUP_RETR, SETUP_RETR_ARD(2)); /* 750 µs delay */

    nRF24L01_vWriteRegister(CONFIG, CONFIG_MASK_EN_CRC(1) | CONFIG_MASK_PWR_UP(1));

    const uint8_t ulTxAddr[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    nRF24L01_vWrite40BitAddressRegister(RX_ADDR_P0, ulTxAddr);
    nRF24L01_vWrite40BitAddressRegister(TX_ADDR, ulTxAddr);

    nRF24L01_vWriteRegister(EN_RXADDR, EN_RXADDR_ERX_P0(1)); /* Enable data pipe 0 */

    while (1)
    {
        ;
    }
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

    const uint8_t ucData[] = { R_REGISTER | ucRegister, R_REGISTER | ucRegister, 0x00 }; /* Add null-terminator for strlen() */

    SPI1_vTransmitDMA((const char *)ucData);
    (void)SPI1_ucReadPolling(); /* Discard status byte */
    ucValue = SPI1_ucReadPolling();
    
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


/**
 * @brief   Write 8-bit value to nRF24L01 address register.
 * 
 * @param   ucRegister      Register to write.
 * 
 * @param   ucValue         Value to write.
 * 
 * @return  None
 */
void nRF24L01_vWrite8BitAddressRegister(const uint8_t ucRegister, const uint8_t ucValue)
{
    nRF24L01_vWriteRegister(ucRegister, ucValue);
}


/**
 * @brief   Write 40-bit value to  nRF24L01 address register.
 * 
 * @note    Blocks task until address written.
 * 
 * @param   ucRegister      Register to write.
 * 
 * @param   ucValue         Value to write.
 * 
 * @return  None
 */
void nRF24L01_vWrite40BitAddressRegister(const uint8_t ucRegister, const uint8_t *pucValue)
{
    uint8_t ucData[RXTX_ADDR_LEN + 2]; /* Pad by 1 on each side */

    /* Build message */
    ucData[0] = W_REGISTER | ucRegister;
    for (uint8_t i = 0; i < RXTX_ADDR_LEN; i++)
    {
        ucData[i + 1] = pucValue[i];
    }
    ucData[RXTX_ADDR_LEN + 1] = 0x00; /* Add null-terminator for strlen() */

    /* First transfer register, then values */
    SPI1_vTransmitDMA((const char *)ucData);

    /* Block task until message sent */
    for (uint8_t i = 0; i < RXTX_ADDR_LEN + 2; i++)
    {
        (void)SPI1_ucReadPolling();
    }
}
