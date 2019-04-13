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


static volatile uint8_t gs_CurrentMode;


/* Function descriptions */
/**
 * Initialize pins used for RF module.
 * Enables PORTE clock.
 */
void RF_Init(void)
{
    /* Enable clock to PORTE */
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Select GPIO for pins */
    PORTE->PCR[ENABLE] = PORT_PCR_MUX(ALT1);
    PORTE->PCR[TXRX] = PORT_PCR_MUX(ALT1);
    PORTE->PCR[DATA_OUT] = PORT_PCR_MUX(ALT1);
    PORTE->PCR[DATA_IN] = PORT_PCR_MUX(ALT1);
    
    FGPIOE->PDDR &= ~MASK(DATA_OUT);
    FGPIOE->PDDR |= MASK(ENABLE) | MASK(TXRX);
    
    RF_SetIdleMode();
}


/* Power consumption 1.2 uA */
void RF_SetPowerdownMode(void)
{
    FGPIOE->PCOR |= MASK(TXRX) | MASK(ENABLE);
    gs_CurrentMode = POWERDOWN_MODE;
}


/* Power consumption 4.7  mA */
void RF_SetIdleMode(void)
{
    PORTE->PCR[DATA_IN] &= ~PORT_PCR_MUX(ALT1);
    FGPIOE->PSOR |= MASK(ENABLE) | MASK(TXRX);
    FGPIOE->PCOR |= MASK(DATA_IN);
    gs_CurrentMode = IDLE_MODE;
}


/* Power consumption 11.5 mA */
void RF_SetTransmissionMode(void)
{
    /* 11.5 mA */
    if (gs_CurrentMode == POWERDOWN_MODE)
    {
        FGPIOE->PSOR |= MASK(ENABLE);
        DelayUs(20);
        FGPIOE->PSOR |= MASK(TXRX);
        DelayUs(400);
    }
    else if (gs_CurrentMode == IDLE_MODE)
    {
        /* No action needed as DATA_IN is driven high during transmission */
        
    }
    else if (gs_CurrentMode == RECEIVER_MODE)
    {
        FGPIOE->PSOR |= MASK(TXRX);
        DelayUs(400);
    }
    else
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    PORTE->PCR[UART0_TX_PIN] = PORT_PCR_MUX(ALT4);
    gs_CurrentMode = TRANSMISSION_MODE;
}


/* Power consumption 6.8 mA */
void RF_SetReceiverMode(void)
{
    if (gs_CurrentMode == POWERDOWN_MODE)
    {
        FGPIOE->PSOR |= MASK(ENABLE);
        DelayUs(20);
        FGPIOE->PSOR |= MASK(TXRX);
        DelayUs(200);
        FGPIOE->PCOR |= MASK(TXRX);
        DelayUs(40);
        FGPIOE->PCOR |= MASK(ENABLE);
        DelayUs(20);
        FGPIOE->PSOR |= MASK(ENABLE);
        DelayUs(200);
    }
    else if (gs_CurrentMode == IDLE_MODE)
    {
        FGPIOE->PCOR |= MASK(TXRX);
        FGPIOE->PSOR |= MASK(ENABLE);
    }
    else if (gs_CurrentMode == TRANSMISSION_MODE)
    {
        /* Make sure TX is done */
        DelayUs(3000);
        
        FGPIOE->PCOR |= MASK(TXRX);
        DelayUs(40);
        FGPIOE->PCOR |= MASK(ENABLE);
        DelayUs(20);
        FGPIOE->PSOR |= MASK(ENABLE);
        DelayUs(200);
    }
    else
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    
    PORTE->PCR[DATA_IN] = PORT_PCR_MUX(ALT1);
    gs_CurrentMode = RECEIVER_MODE;
}
