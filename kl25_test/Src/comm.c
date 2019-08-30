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
                (void)nRF24L01_ucReadRegister(0x05);
                nRF24L01_vWriteRegister(0x05, 0x54);
                (void)nRF24L01_ucReadRegister(0x05);

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
 * @brief   Sets SPI1 SS line high and stops DMA0.
 * 
 * @param   None
 * 
 * @return  None
 */
void TPM2_IRQHandler(void)
{
    if (BME_UBFX32(&TPM2->STATUS, TPM_STATUS_TOF_SHIFT, 1))
    {
        SPI1_vSetSlave(HIGH);

        TPM2_vStop();

        /* Reset counters */
        TPM2->CNT = 0;

        /* Disable DMA transmitter */
        BME_AND8(&SPI1->C2, ~(uint8_t)SPI_C2_TXDMAE(1));

        DMA0_vStop();

        /* Clear DONE & error bits */
        BME_OR32(&DMA0->DMA[0].DSR_BCR, DMA_DSR_BCR_DONE(1));
    }

    /* Clear Timer Overflow Flag */
    BME_OR32(&TPM2->STATUS, TPM_STATUS_TOF(1));
}
