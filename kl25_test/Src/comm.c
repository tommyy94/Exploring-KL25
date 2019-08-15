#include "comm.h"


/* Local defines */

/* Global variables */
QueueHandle_t xCommQueue;
SemaphoreHandle_t xCommSemaphore;

struct AMessage
{
    uint32_t ulCrc32;
    char ucCrc32Frame[MAX_FRAME_SIZE];
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
                /* SEND MESSAGE THROUGH SPI */
                
                /* This call should not fail in any circumstance */
                xAssert = xSemaphoreGive(xCommSemaphore);
                configASSERT(xAssert == pdTRUE);
            }
        }
        
        vTaskDelay(MSEC_TO_TICK(50));
    }
}


/**
 * @brief   FreeRTOS SQL task.
 * 
 * @param   pvParam     Unused.
 * 
 * @return  None
 */
void vSqlTask(void *const pvParam)
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
                /* Build POST request */
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
