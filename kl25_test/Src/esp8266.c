#include "esp8266.h"


#define BUFFLEN     (255UL)


uint8_t UART0_ucRxBuffer[BUFFLEN];


static void ESP8266_vFlushBuffer(char *const buffer);


/**
 * Reset and initialize ESP8266.
 * 
 * @param   None
 * 
 * @return  None
 */
void ESP8266_vInit(void)
{
    ESP8266_vSendCmd(AT_RESET);
    ESP8266_vSendCmd(AT_STA_MODE);
    ESP8266_vSendCmd(AT_CONNECT_AP);
}


/**
 * Send command to ESP8266.
 * 
 * @param   pcCmd     AT command as string.
 * 
 * @return  None
 */
void ESP8266_vSendCmd(char *const pcCmd)
{
    const TickType_t xTimeoutTicks = 10 / portTICK_PERIOD_MS;
    const char ucAckMsg[] = "OK";
    
    for (uint8_t tries = 5; tries > 0; tries--)
    {
        /* Start transmission */
        DMA0_vStart((uint32_t *)UART0_ucRxBuffer, BUFFLEN);
        UART0_vTransmitPolling(pcCmd);
        
        /* Wait for response */
        vTaskDelay(xTimeoutTicks);
        
        /* Proceed if OK was received */
        if (strstr((const char *)UART0_ucRxBuffer, ucAckMsg) != NULL)
        {
            break;
        }
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
        pcBuffer[i] = '\0';
    }
}
