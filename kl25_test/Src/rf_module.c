#include "rf_module.h"


/* Local defines */
#define DATA_IN     (20UL)
#define DATA_OUT    (21UL)
#define ENABLE      (22UL)
#define TXRX        (23UL)


/* Define operational modes for the RF module. */
enum OperationalModes
{
    POWERDOWN_MODE,
    IDLE_MODE,
    TRANSMISSION_MODE,
    RECEIVER_MODE
};


static uint8_t ucCurrentMode;


/* Function descriptions */

/**
 * @brief   Initialize pins used for RF module.
 * 
 * @param   None
 * 
 * @return  None
 * @note    Uses PORTE.
 */
void RF_vInit(void)
{
    /* Enable clock to PORTE */
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Select GPIO for pins */
    PORTE->PCR[ENABLE] = PORT_PCR_MUX(ALT1);
    PORTE->PCR[TXRX] = PORT_PCR_MUX(ALT1);
    //PORTE->PCR[DATA_OUT] = PORT_PCR_MUX(ALT1);
    PORTE->PCR[DATA_IN] = PORT_PCR_MUX(ALT1);
    
    //FGPIOE->PDDR &= ~MASK(DATA_OUT);
    FGPIOE->PDDR |= MASK(ENABLE) | MASK(TXRX);
    
    RF_vSetIdleMode();
}


/**
 * @brief   Set powerdown mode for RF module (current consumption 1.2 uA).
 * 
 * @param   None
 * 
 * @return  None
 */
void RF_vSetPowerdownMode(void)
{
    FGPIOE->PCOR |= MASK(TXRX) | MASK(ENABLE);
    ucCurrentMode = POWERDOWN_MODE;
}


/**
 * @brief   Set idle mode for RF module (current consumption 4.7 mA).
 * 
 * @param   None
 * 
 * @return  None
 */
void RF_vSetIdleMode(void)
{
    PORTE->PCR[DATA_IN] &= ~PORT_PCR_MUX(ALT1);
    FGPIOE->PSOR |= MASK(ENABLE) | MASK(TXRX);
    FGPIOE->PCOR |= MASK(DATA_IN);
    ucCurrentMode = IDLE_MODE;
}


/**
 * @brief   Set transmission mode for RF module (current consumption 11.5 mA).
 * 
 * @param   None
 * 
 * @return  None
 * @note    FreeRTOS Scheduler must be started before using this function.
 */
void RF_vSetTransmissionMode(void)
{
    if (ucCurrentMode == POWERDOWN_MODE)
    {
        FGPIOE->PSOR |= MASK(ENABLE);
        vTaskDelay(20);
        FGPIOE->PSOR |= MASK(TXRX);
        vTaskDelay(400);
    }
    else if (ucCurrentMode == IDLE_MODE)
    {
        /* No action needed as DATA_IN is driven high during transmission */
    }
    else if (ucCurrentMode == RECEIVER_MODE)
    {
        FGPIOE->PSOR |= MASK(TXRX);
        vTaskDelay(400);
    }
    else
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    PORTE->PCR[UART0_TX_PIN] = PORT_PCR_MUX(ALT4);
    ucCurrentMode = TRANSMISSION_MODE;
}


/**
 * @brief   Set receiver mode for RF module (current consumption 6.8 mA).
 * 
 * @param   None
 * 
 * @return  None
 * @note    FreeRTOS Scheduler must be started before using this function.
 */
void RF_vSetReceiverMode(void)
{
    if (ucCurrentMode == POWERDOWN_MODE)
    {
        FGPIOE->PSOR |= MASK(ENABLE);
        vTaskDelay(20);
        FGPIOE->PSOR |= MASK(TXRX);
        vTaskDelay(200);
        FGPIOE->PCOR |= MASK(TXRX);
        vTaskDelay(40);
        FGPIOE->PCOR |= MASK(ENABLE);
        vTaskDelay(20);
        FGPIOE->PSOR |= MASK(ENABLE);
        vTaskDelay(200);
    }
    else if (ucCurrentMode == IDLE_MODE)
    {
        FGPIOE->PCOR |= MASK(TXRX);
        FGPIOE->PSOR |= MASK(ENABLE);
    }
    else if (ucCurrentMode == TRANSMISSION_MODE)
    {
        /* Make sure TX is done */
        vTaskDelay(3000);
        
        FGPIOE->PCOR |= MASK(TXRX);
        vTaskDelay(40);
        FGPIOE->PCOR |= MASK(ENABLE);
        vTaskDelay(20);
        FGPIOE->PSOR |= MASK(ENABLE);
        vTaskDelay(200);
    }
    else
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    PORTE->PCR[DATA_IN] = PORT_PCR_MUX(ALT1);
    ucCurrentMode = RECEIVER_MODE;
}
