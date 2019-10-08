/**
 * nrf24l01.c
 * Driver for NRF24l01.
 */

#include "nrf24l01.h"


/* Local defines */
#define CE                          (1UL)       /* Chip Enable */
#define IRQ                         (2UL)       /* Interrupt Request */

#define RXTX_ADDR_LEN               (5UL)
#define MAX_PAYLOAD_LEN             (32UL)
#define ADDR_40BIT_LEN              (6UL)

/* Commands */
#define R_REGISTER                  (0x00UL)    /* Read command and status registers */
#define W_REGISTER                  (0x20UL)    /* Write command and status registers - power down/standby modes only */
#define R_RX_PAYLOAD                (0x61UL)    /* Read RX-payload */
#define W_TX_PAYLOAD                (0xA0UL)    /* Write TX-payload */
#define FLUSH_TX                    (0xE1UL)    /* Flush TX FIFO */
#define FLUSH_RX                    (0xE2UL)    /* Flush RX FIFO */
#define REUSE_TX_PL                 (0xE3UL)    /* Reuse last transmitted payload */
#define R_RX_PL_WID                 (0x63UL)    /* Read RX payload width */
#define W_ACK_PAYLOAD               (0xA8UL)    /* Write Payload to be transmitted together ACK packet */
#define W_ACK_PAYLOAD_NOACK         (0xB0UL)    /* Disable AUTOACK in specific packet */
#define NOP                         (0xFFUL)    /* No Operation to read STATUS register */

/* Registers */
#define CONFIG                      (0x00UL)    /* Configuration Register */
#define EN_AA                       (0x01UL)    /* Enable Auto Acknowledgement */
#define EN_RXADDR                   (0x02UL)    /* Enabled RX Addresses */
#define SETUP_RETR                  (0x04UL)    /* Setup of Automatic Retransmission */
#define RF_CH                       (0x05UL)    /* RF Channel */
#define RF_SETUP                    (0x06UL)    /* RF Setup Register */
#define STATUS                      (0x07UL)    /* Status Register */
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
#define CONFIG_MASK_RX_DR(x)        (((uint8_t)(((uint8_t)(x)) << 6)) & 0x40UL)
#define CONFIG_MASK_TS_DS(x)        (((uint8_t)(((uint8_t)(x)) << 5)) & 0x20UL)
#define CONFIG_MASK_MAX_RT(x)       (((uint8_t)(((uint8_t)(x)) << 4)) & 0x10UL)
#define CONFIG_EN_CRC(x)            (((uint8_t)(((uint8_t)(x)) << 3)) & 0x08UL)
#define CONFIG_CRCO(x)              (((uint8_t)(((uint8_t)(x)) << 2)) & 0x04UL)
#define CONFIG_PWR_UP(x)            (((uint8_t)(((uint8_t)(x)) << 1)) & 0x02UL)
#define CONFIG_PRIM_RX(x)           (((uint8_t)(((uint8_t)(x)) << 0)) & 0x01UL)

#define EN_AA_ENAA_P5(x)            (((uint8_t)(((uint8_t)(x)) << 5)) & 0x20UL)
#define EN_AA_ENAA_P4(x)            (((uint8_t)(((uint8_t)(x)) << 4)) & 0x10UL)
#define EN_AA_ENAA_P3(x)            (((uint8_t)(((uint8_t)(x)) << 3)) & 0x08UL)
#define EN_AA_ENAA_P2(x)            (((uint8_t)(((uint8_t)(x)) << 2)) & 0x04UL)
#define EN_AA_ENAA_P1(x)            (((uint8_t)(((uint8_t)(x)) << 1)) & 0x02UL)
#define EN_AA_ENAA_P0(x)            (((uint8_t)(((uint8_t)(x)) << 0)) & 0x01UL)

#define EN_RXADDR_ERX_P5(x)         (((uint8_t)(((uint8_t)(x)) << 5)) & 0x20UL)
#define EN_RXADDR_ERX_P4(x)         (((uint8_t)(((uint8_t)(x)) << 4)) & 0x10UL)
#define EN_RXADDR_ERX_P3(x)         (((uint8_t)(((uint8_t)(x)) << 3)) & 0x08UL)
#define EN_RXADDR_ERX_P2(x)         (((uint8_t)(((uint8_t)(x)) << 2)) & 0x04UL)
#define EN_RXADDR_ERX_P1(x)         (((uint8_t)(((uint8_t)(x)) << 1)) & 0x02UL)
#define EN_RXADDR_ERX_P0(x)         (((uint8_t)(((uint8_t)(x)) << 0)) & 0x01UL)

#define SETUP_RETR_ARD(x)           (((uint8_t)(((uint8_t)(x)) << 4)) & 0xF0UL)
#define SETUP_RETR_ARC(x)           (((uint8_t)(((uint8_t)(x)) << 0)) & 0x0FUL)

#define STATUS_RX_DR(x)             (((uint8_t)(((uint8_t)(x)) << 6)) & 0x40UL)
#define STATUS_TX_DS(x)             (((uint8_t)(((uint8_t)(x)) << 5)) & 0x20UL)
#define STATUS_MAX_RT(x)            (((uint8_t)(((uint8_t)(x)) << 4)) & 0x10UL)
#define STATUS_RX_P_NO(x)           (((uint8_t)(((uint8_t)(x)) << 1)) & 0x0EUL)
#define STATUS_TX_FULL(x)           (((uint8_t)(((uint8_t)(x)) << 0)) & 0x01UL)

#define RF_CH_MHZ(x)                (((uint8_t)(((uint8_t)(x)) << 0)) & 0x7FUL)

#define RF_SETUP_CONT_WAVE(x)       (((uint8_t)(((uint8_t)(x)) << 7)) & 0x80UL)
#define RF_SETUP_RF_DR_LOW(x)       (((uint8_t)(((uint8_t)(x)) << 5)) & 0x30UL)
#define RF_SETUP_PLL_LOCK(x)        (((uint8_t)(((uint8_t)(x)) << 4)) & 0x10UL)
#define RF_SETUP_RF_DR_HIGH(x)      (((uint8_t)(((uint8_t)(x)) << 3)) & 0x08UL)
#define RF_SETUP_RF_PWR(x)          (((uint8_t)(((uint8_t)(x)) << 1)) & 0x06UL)

#define RX_PW_PX(x)                 (((uint8_t)(((uint8_t)(x)) << 0)) & 0x3FUL)


/* Local function prototypes */
__STATIC_INLINE void nRF24L01_vConfigureIRQ(void);
__STATIC_INLINE void nRF24L01_vConfigureChipEnable(void);
__STATIC_INLINE void nRF24L01_vSetChipEnable(const uint32_t ulState);
static void nRF24L01_vStartTransmission(void);


/* Function descriptions */

/**
 * @brief   Initialize nRF24L01 and masters peripherals.
 * 
 * @param   None
 * 
 * @return  None
 */
void nRF24L01_vInit(void)
{
    nRF24L01_vConfigureIRQ();
    nRF24L01_vConfigureChipEnable();
    nRF24L01_vSetChipEnable(LOW);

    /* RF Channel 2450 MHz */
    nRF24L01_vWriteRegister(RF_CH, RF_CH_MHZ(50));

    /* Set RX & TX address matching */
    const uint8_t ucTxAddr[ADDR_40BIT_LEN] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x00 }; /* LSB written first, null-terminator at end */
    nRF24L01_vWriteAddressRegister(RX_ADDR_P0, ucTxAddr);
    nRF24L01_vWriteAddressRegister(TX_ADDR, ucTxAddr);
    
    /* Enable data pipe 0 */
    nRF24L01_vWriteRegister(EN_RXADDR, EN_RXADDR_ERX_P0(1));

    /* Auto ACK data pipe 0 */
    nRF24L01_vWriteRegister(EN_AA, EN_AA_ENAA_P0(1));

    /**
     * 500 탎 delay between retries
     * 10 retries
     */
    nRF24L01_vWriteRegister(SETUP_RETR, SETUP_RETR_ARD(1) | SETUP_RETR_ARC(3));

    /**
     * Enable CRC
     * 2 byte CRC
     * Power Up
     * TX mode
     */
    nRF24L01_vWriteRegister(CONFIG, CONFIG_EN_CRC(1) | CONFIG_CRCO(1) | CONFIG_PWR_UP(1));
}


/**
 * @brief   Configure IRQ pin.
 * 
 * @param   None
 *             
 * @return  None
 */
__STATIC_INLINE void nRF24L01_vConfigureIRQ(void)
{
    /**
     * Select GPIO
     * Interrupt on falling edge
     * Enable internal pullup resistor
     */
    PORTA->PCR[IRQ] = PORT_PCR_MUX(ALT1) | PORT_PCR_IRQC(10) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
    
    /* Configure NVIC */
    NVIC_SetPriority(PORTA_IRQn, 2);
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    NVIC_EnableIRQ(PORTA_IRQn);
}


/**
 * @brief   Configure CE pin as output.
 * 
 * @param   None
 *             
 * @return  None
 */
__STATIC_INLINE void nRF24L01_vConfigureChipEnable(void)
{
    /* Select GPIO */
    PORTA->PCR[CE] = PORT_PCR_MUX(ALT1);
    
    /* Set output */
    FGPIOA->PDDR |= MASK(CE);    
}


/**
 * @brief   Set CE line high/low.
 * 
 * @param   ulState     HIGH/LOW
 *             
 * @return  None
 */
__STATIC_INLINE void nRF24L01_vSetChipEnable(const uint32_t ulState)
{
    configASSERT(ulState == LOW || (ulState == HIGH));

    /* Figure out whether to set or clear bit */
    const uint32_t *pulReg = (uint32_t *)&FGPIOA->PCOR - ulState; /* Subtract 0 - 1 words from PCOR address => pulReg = FGPIOA->PSOR/PCOR */

    /* Perform bitwise operation */
    BME_OR32(&(*pulReg), MASK(CE));
}


/**
 * @brief   Pulse CE line low for 10 탎 to start transmission.
 * 
 * @param   None
 * 
 * @return  None
 */
static void nRF24L01_vStartTransmission(void)
{
    /* Disable TPM2 interrupts just to be sure */
    BME_AND8(&TPM2->SC, ~(uint8_t)TPM_SC_TOIE(1));

    /* Send 10 탎 pulse */
    nRF24L01_vSetChipEnable(HIGH);
    TPM2_vStart();

    //vTaskDelay(10);
    while (TPM2->CNT < TEN_MICROSECONDS)
    {
        ; /* Wait until 10 탎 passed */
    }

    nRF24L01_vSetChipEnable(LOW);
    TPM2_vStop();

    /* Turn TPM2 interrupts on again */
    BME_OR8(&TPM2->SC, TPM_SC_TOIE(1));

    /* Reset TPM2 counter */
    TPM2->CNT = 0;
}


/**
 * @brief   Reset the given mask of status bits.
 * 
 * @param   ucStatusMask    Status bits to reset.
 *             
 * @return  None
 */
void nRF24L01_vResetStatusFlags(const uint8_t ucStatusMask)
{
    nRF24L01_vWriteRegister(STATUS, ucStatusMask);
}


/**
 * @brief   Transmit payload.
 * 
 * @note    Message max length 32 bytes.
 * 
 * @param   pucPayload      Payload to send.
 *             
 * @return  None
 */
void nRF24L01_vSendPayload(const char *pucPayload)
{
    const uint32_t ulPayloadLength = strlen((const char *)pucPayload) + 1; /* Add W_TX_PAYLOAD as first element later */
    configASSERT((ulPayloadLength - 1) <= MAX_PAYLOAD_LEN); /* W_TX_PAYLOAD not included in the payload */
    uint8_t ucData[ulPayloadLength + 1]; /* Allocate space for null-terminator */

    /* Transfer 1...32 bytes */
    nRF24L01_vWriteRegister(RX_PW_P0, RX_PW_PX(ulPayloadLength));

    /* Flush TX buffer */
    nRF24L01_vSendCommand(FLUSH_TX);

    /**
     * Reset data received flag
     * Reset transmission succeeded flag
     * Reset transmission failed flag
     */
    nRF24L01_vResetStatusFlags(STATUS_RX_DR(1) | STATUS_TX_DS(1) | STATUS_MAX_RT(1));

    /* Build message */
    ucData[0] = W_TX_PAYLOAD;
    for (uint32_t i = 0; i < ulPayloadLength - 1; i++) /* First index already filled */
    {
        ucData[i + 1] = pucPayload[i];
    }
    ucData[ulPayloadLength] = 0x00; /* Add null-terminator for strlen() */

    /* Transfer bytes to nRF24L01 */
    SPI1_vTransmitDMA((const char *)ucData);
    
    nRF24L01_vStartTransmission();
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
 * 
 * @deprecated              Don't use.
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
 * @brief   Write command nRF24L01.
 * 
 * @param   ucCommand
 * 
 * @return  None
 */
void nRF24L01_vSendCommand(const uint8_t ucCommand)
{
    const uint8_t ucCommandString[] = { ucCommand, 0x00 }; /* Add null-terminator for strlen() */

    SPI1_vTransmitDMA((const char *)ucCommandString);
}


/**
 * @brief   Write to nRF24L01 address register.
 * 
 * @note    Blocks task until address written.
 * 
 * @param   ucRegister      Register to write.
 * 
 * @param   ucValue         Value to write.
 * 
 * @return  None
 */
void nRF24L01_vWriteAddressRegister(const uint8_t ucRegister, const uint8_t *pucValue)
{
    const uint32_t ulLength = strlen((const char *)pucValue) + 1; /* Add W_REGISTER as first element later */
    configASSERT((ulLength - 1) <= ADDR_40BIT_LEN);  /* W_REGISTER not included in the payload */

    uint8_t ucData[ulLength];

    /* Build message */
    ucData[0] = W_REGISTER | ucRegister;
    for (uint32_t i = 0; i < ulLength - 1; i++) /* First index already filled */
    {
        ucData[i + 1] = pucValue[i];
    }

    /* First transfer register, then values */
    SPI1_vTransmitDMA((const char *)ucData);
}


/**
 * @brief   PORTA IRQ handler. Triggered when nRF24L01 transmits payload.
 * 
 * @param   None
 * 
 * @return  None
 */
void PORTA_IRQHandler(void)
{
    if (PORTA_ISFR & MASK(IRQ))
    {
        /* Clear status flag */
        PORTA->ISFR = MASK(IRQ);
    }
}
