#include "esp8266.h"


#define BUFFLEN     (255UL)


uint8_t UART0_ucRxBuffer[BUFFLEN];


static void ESP8266_vFlushBuffer(char *const buffer);


/**
 * Reset ESP8266.
 * 
 * @param   None
 * 
 * @return  None
 * @todo Implement some sort of timeout for retransmits. (Remove vAssertCalled()).
 */
void ESP8266_vInit(TimerHandle_t *const pxTimeoutTimers)
{
    ESP8266_vSendCmd(AT_RESET, pxTimeoutTimers);
    ESP8266_vSendCmd(AT_STA_MODE, pxTimeoutTimers);
    ESP8266_vSendCmd(AT_CONNECT_AP, pxTimeoutTimers);
}


/**
 * Send command to ESP8266.
 * 
 * @param   pcCmd     AT command as string.
 * 
 * @return  None
 * @todo Implement some sort of timeout for retransmits. (Remove vAssertCalled()).
 */
void ESP8266_vSendCmd(char *const pcCmd, TimerHandle_t *const pxTimeoutTimers)
{
    BaseType_t xAssert;
    const char ucAckMsg[] = "OK";
    
    DMA0_vStart((uint32_t *)UART0_ucRxBuffer, 255);
    UART0_vTransmitPolling(pcCmd);
    
    /* Start software timer */
    xAssert = xTimerStart(pxTimeoutTimers[0], (TickType_t)0);
    configASSERT(xAssert);
    
    if (strstr((const char *)UART0_ucRxBuffer, ucAckMsg) == NULL)
    {
        vAssertCalled(__LINE__, __FILE__);
    }
    
    ESP8266_vFlushBuffer((char *const)UART0_ucRxBuffer);
    
}


/**
 * Flush RX buffer.
 * 
 * @param   pcBuffer    Buffer to flush.
 * 
 * @return  None
 */
static void ESP8266_vFlushBuffer(char *const pcBuffer)
{
    for (uint32_t i = 0; i < BUFFLEN; i++)
    {
        pcBuffer[i] = 0;
    }
}
