#include "comm.h"


/* Local defines */

/* Global variables */
QueueHandle_t xCommQueue;
SemaphoreHandle_t xCommSemaphore;

struct AMessage
{
    char ucFrame[MAX_FRAME_SIZE];
} xMessage;


/* Function descriptions */

/**
 * @brief   FreeRTOS communication task.
 * 
 * @param   pvParam     Unused.
 * 
 * @return  None
 */
void vCommTask(void *const pvParam)
{
    (void)pvParam;
    BaseType_t xAssert;
    struct AMessage *pxMessage;
    const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
    
    for (;;)
    {
        if (xQueueReceive(xCommQueue, &(pxMessage), (TickType_t) 10))
        {
            /* Used to guard RF modules state in future */
            if (xSemaphoreTake(xCommSemaphore, (TickType_t)xTicksToWait))
            {
                SPI1_vTransmitDMA(pxMessage->ucFrame);
                //volatile uint32_t tmp = nRF24L01_ulReadRegister(0x04);
                
                /* This call should not fail in any circumstance */
                xAssert = xSemaphoreGive(xCommSemaphore);
                configASSERT(xAssert == pdTRUE);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(50));
    }
}


/**
 * @brief   FreeRTOS frame task. Builds the message to send.
 * 
 * @param   pvParam     Unused.
 * 
 * @return  None
 */
void vFrameTask(void *const pvParam)
{
    (void)pvParam;
    int8_t cBytesWritten;
    BaseType_t xAssert;
    
    struct Sensor *pxSensor;
    struct AMessage *pxMessage;
    pxMessage = &xMessage;
    
    for (;;)
    {
        if (xAnalogQueue != 0)
        {
            if (xQueueReceive(xAnalogQueue, &pxSensor, (TickType_t)50))
            {
                /* Build the frame */
                cBytesWritten = csnprintf(pxMessage->ucFrame, MAX_FRAME_SIZE, "tmp=23;hum=50;soil=30;");
                //cBytesWritten = csnprintf(pxMessage->ucFrame, MAX_FRAME_SIZE, "tmp=%ldhum=%lumst=%lu", pxSensor->lTemperature, pxSensor->ulHumidity, pxSensor->ulSoilMoisture);
                configASSERT(cBytesWritten >= 0);
            
                /* Transmit */
                xAssert = xQueueSend(xCommQueue, (void *)&pxMessage, (TickType_t)10);
                configASSERT(xAssert);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(100));
    }
}
