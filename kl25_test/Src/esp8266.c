#include "esp8266.h"


uint8_t UART0_ucRxBuffer[255];


/**
 * Reset ESP8266.
 * 
 * @param   None
 * 
 * @return  None
 * @todo Maybe implement strnstr().
 * @todo Implement some sort of timeout for retransmits. (Remove vAssertCalled()).
 */
void ESP8266_vInit(void)
{
    const char ucAckMsg[] = "OK";
    
    DMA0_vStart((uint32_t *)UART0_ucRxBuffer, 255);
    UART0_vTransmitPolling(AT_RESET);
    
    if (strstr((const char *)UART0_ucRxBuffer, ucAckMsg) == NULL)
    {
        vAssertCalled(__LINE__, __FILE__);
    }
}


/**
 * Send command to ESP8266.
 * 
 * @param   pcCmd     AT command as string.
 * 
 * @return  None
 * @todo Maybe implement strnstr().
 * @todo Implement some sort of timeout for retransmits. (Remove vAssertCalled()).
 */
void ESP8266_vSendCmd(const char *pcCmd)
{
    const char ucAckMsg[] = "OK";
    
    DMA0_vStart((uint32_t *)UART0_ucRxBuffer, 255);
    UART0_vTransmitPolling(pcCmd);
    
    if (strstr((const char *)UART0_ucRxBuffer, ucAckMsg) == NULL)
    {
        vAssertCalled(__LINE__, __FILE__);
    }
    
}
