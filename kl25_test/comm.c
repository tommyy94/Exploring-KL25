#include "comm.h"


/* Local defines */
#define UART0_OVERSAMPLE_RATE   (16UL)
#define UART0_MAX_MSG_LEN       (128UL)

/* Global variables */
uint8_t ucRxData[UART0_RX_BUFSIZ];
uint8_t ucRxFlag = false;
volatile uint8_t ucRxIndex = 0;
QueueHandle_t xCommQueue;

struct AMessage
{
    uint32_t ulCrc32;
    char ucCrc32Frame[MAX_FRAME_SIZE];
    char ucFrame[MAX_FRAME_SIZE];
} xMessage;



/* Function descriptions */
void UART0_Init(const uint32_t baudrate)
{
    register uint16_t sbr;
    
    /* Enable clock gating for UART0 & PORTE */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Disable transmitter & receiver */
    UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;
    
    /* Set UART0 clock to 48 MHz */
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    /* Set pins to UART0 TX & RX */
    PORTE->PCR[UART0_TX_PIN] = PORT_PCR_MUX(ALT4);
    PORTE->PCR[UART0_RX_PIN] = PORT_PCR_MUX(ALT4);
    
    /**
     * Set baudrate
     * Set oversampling ratio
     */
    sbr = (uint16_t)(SystemCoreClock / (baudrate * UART0_OVERSAMPLE_RATE));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0->BDL = UART0_BDL_SBR(sbr);
    UART0->C4 |= UART0_C4_OSR(UART0_OVERSAMPLE_RATE - 1);
    
    /* Select one stop bit */
    UART0->BDH |= UART0_BDH_SBNS(0);
    
    /**
     * Disable loopback mode
     * 8-bit mode
     * No parity bits
     */
    UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
    
    /**
     * Don't invert TX data
     * Enable interrupts for errors
     */
    UART0->C3 = UART0_C3_TXINV(0) |UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1) | UART0_C3_PEIE(1);
    
    /* Clear error flags */
    UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
    
    /**
     * Send LSB first
     * Don't invert RX data
     */
    UART0->S2 = UART0_S2_MSBF(0) |UART0_S2_RXINV(0);
    
    /**
     * Enable TX & RX
     * Enable RX interrupts
     */
    UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1) |UART0_C2_RIE(1);
    
    NVIC_SetPriority(UART0_IRQn, 2);
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);
}


uint32_t UART0_ReadPolling(void)
{
    while (!(UART0->S1 & UART0_S1_RDRF_MASK))
    {
        ; /* Wait for data */
    }
    
    return (UART0->D);
}


void UART0_IRQHandler(void)
{
    if (UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
    {
        /* Clear error flags */
        UART0->S1 |= UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK;
        
        /* Clear RDRF */
        (void)UART0->D;
    }
    
    if (UART0->S1 & UART_S1_RDRF_MASK)
    {
        ucRxData[ucRxIndex++] = UART0->D;
        if (ucRxIndex >= UART0_RX_BUFSIZ)
        {
            ucRxIndex = 0;
            ucRxFlag = true;
        }
    }
}


void UART0_TransmitByte(const char byte)
{
    while (!(UART0->S1 & UART0_S1_TDRE_MASK))
    {
        ; /* Wait until TX buffer empty */
    } 
        
    /* Send character */
    UART0->D = byte;
}


void UART0_TransmitPolling(const char *data)
{
    /* Find size of array */
    uint16_t dataLen = (uint16_t)strlen(data);
    
    /* Send the array of characters */
    for (uint16_t i = 0; i < dataLen; i++)
    {
        UART0_TransmitByte(data[i]);
    }
}


void vCommTask(void *const param)
{
    (void)param;
    struct AMessage *pxMessage;
    
    for (;;)
    {
        if (xQueueReceive(xCommQueue, &(pxMessage), (TickType_t) 10))
        {
            UART0_TransmitPolling(pxMessage->ucFrame);
        }
        else
        {
            UART0_TransmitPolling("Error receiving from commQueue!\004");
        }
        
        vTaskDelay(MSEC_TO_TICK(100));
    }
}


void vCrcTask(void *const param)
{
    (void)param;
    struct Sensor *pxSensor;
    struct AMessage *pxMessage;
    pxMessage = &xMessage;
    
    for (;;)
    {
        if (xAnalogQueue != 0)
        {
            if (xQueueReceive(xAnalogQueue, &pxSensor, (TickType_t)10))
            {
                /* Build the frame with checksum */
                //snprintf(pxMessage->ucData, MAX_FRAME_SIZE, "abcdefgh0123456789"); /* For test purposes */
                snprintf(pxMessage->ucFrame, MAX_FRAME_SIZE, "tmp=%luhum=%lumst=%lu", pxSensor->ulTemperature, pxSensor->ulHumidity, pxSensor->ulSoilMoisture);
            }
            else
            {
                strncpy(pxMessage->ucFrame, "Error receiving from analogQueue!", 27);
            }
            
            pxMessage->ulCrc32 = CRC_Fast((uint8_t *)pxMessage->ucFrame, strlen(pxMessage->ucFrame));
            snprintf(pxMessage->ucCrc32Frame, MAX_FRAME_SIZE, "crc32:%x\004", (unsigned int)pxMessage->ulCrc32);
            strncat(pxMessage->ucFrame, pxMessage->ucCrc32Frame, strlen(pxMessage->ucCrc32Frame));
            
            if (xQueueSend(xCommQueue, (void *)&pxMessage, (TickType_t)10) != pdPASS)
            {
                vErrorHandler(__FILE__, __LINE__);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(500));
    }
}
