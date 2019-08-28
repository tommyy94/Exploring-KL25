#include "comm.h"


/* Local defines */

/* Global variables */
QueueHandle_t xCommQueue;
SemaphoreHandle_t xCommSemaphore;
static TaskHandle_t xCommTaskHandle;

struct AMessage
{
    char ucFrame[MAX_FRAME_SIZE];
} xMessage;

/* Local function prototypes */
static void vWaitUntilTransferCplt(void);
    
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
    xCommTaskHandle = xTaskGetCurrentTaskHandle();
    
    for (;;)
    {
        if (xQueueReceive(xCommQueue, &(pxMessage), (TickType_t) 10))
        {
            /* Used to guard RF modules state in future */
            if (xSemaphoreTake(xCommSemaphore, (TickType_t)xTicksToWait))
            {                
                /* Begin transfer */
                SPI1_vSetSlave(LOW);
                
                nRF24L01_vWriteRegister(0x05, 0x67);
                //SPI1_vTransmitDMA(pxMessage->ucFrame);
                
                /* Block task until all bytes sent */
                vWaitUntilTransferCplt();
                
                /* End transfer */
                SPI1_vSetSlave(HIGH);
                
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


/**
 * @brief   DMA0 IRQ Handler for transaction complete.
 * 
 * @note    This handler triggers after last byte is moved to SPI TX register.
 *          The Slave Select line can't be pulled HIGH afterwards, because
 *          the byte is still being transmitted so some delay is needed.
 *          Delay introduced by sending task notification should be
 *          sufficient in this case.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_IRQHandler(void)
{
    /**
     * Check for:
     *  Configuration error
     *  Bus error on source
     *  Bus error on destination
     */
    configASSERT(DMA0->DMA[0].DSR_BCR & ~(DMA_DSR_BCR_CE(1) & DMA_DSR_BCR_BES(1) & DMA_DSR_BCR_BED(1)));
    
    DMA0_vStop();
    
    /* Clear DONE & error bits */
    BME_OR32(&DMA0->DMA[0].DSR_BCR, DMA_DSR_BCR_DONE(1));
    
    /*-----------------------------------------------------------*/
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    /* Send transfer complete signal */
    vTaskNotifyGiveFromISR(xCommTaskHandle, &xHigherPriorityTaskWoken);
    
    /* Force context switch if xHigherPriorityTaskWoken is set to pdTRUE */
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);   
}


/**
 * @brief   This function blocks callee task until SPI transfer is completed.
 * 
 * @param   None
 * 
 * @return  None
 */
static void vWaitUntilTransferCplt(void)
{
    BaseType_t xAssert;
    const TickType_t xTicksToWait = 10 / portTICK_PERIOD_MS;
    
    /* Wait until transfer done */
    xAssert = ulTaskNotifyTake(pdTRUE, xTicksToWait);
    configASSERT(xAssert == pdPASS);
}
