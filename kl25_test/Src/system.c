#include "system.h"


void vSystemInit(void)
{
    /* Analog functionalities */
    ADC0_vInit();
    TPM0_vInit(4800);
    TPM1_vInit();
    CMP0_vInit();
    HS1101_vInit();
    
    /* Communications */
    UART0_vInit(9600);
    CRC_vInit();
//    RF_vInit();
    
//    GPIO_vInit();
}


void vCreateQueues(void)
{
    xAnalogQueue = xQueueCreate(MAX_QUEUE_SIZE, sizeof(struct Sensor));
    if (xAnalogQueue == NULL)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    xCommQueue = xQueueCreate(MAX_QUEUE_SIZE, sizeof(char *));
    if (xCommQueue == NULL)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


EventGroupHandle_t xMotorEventGroup;
void vCreateEvents(void)
{
    xMotorEventGroup = xEventGroupCreate();
    if (xMotorEventGroup == NULL)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


void vCreateTasks(void *pvParameters)
{
    TaskHandle_t xHandle;
    
    if (xTaskCreate(vCrcTask, (const char *)"CRC", CRCTASKSIZE / sizeof(portSTACK_TYPE), 0, CRCTASKPRIORITY, &xHandle) != pdPASS)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    if (xTaskCreate(vSensorTask, (const char *)"Sensor", ANALOGTASKSIZE / sizeof(portSTACK_TYPE), 0, ANALOGTASKPRIORITY, &xHandle) != pdPASS)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    if (xTaskCreate(vCommTask, (const char *)"Commu", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &xHandle) != pdPASS)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
    
    if (xTaskCreate(vMotorTask, (const char *)"Motor", MOTORTASKSIZE / sizeof(portSTACK_TYPE), pvParameters, MOTORTASKPRIORITY, &xHandle) != pdPASS)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


void vCreateTimers(TimerHandle_t *pxTimers)
{
    for (uint32_t i = 0; i < MOTOR_COUNT; i++)
    {
        /* TODO: Format timer names */
        pxTimers[i] = xTimerCreate("Motor Timer", pdMS_TO_TICKS(100), pdTRUE, (void *)i, vTimerCallback);
        if (pxTimers[i] == NULL)
        {
            vErrorHandler(__FILE__, __LINE__);
        }
    }
}


void vTimerCallback(TimerHandle_t xTimer)
{
    EventBits_t uxBits;
    const uint32_t xTimerId = (uint32_t)pvTimerGetTimerID(xTimer);
    
    uxBits = xEventGroupSetBits(xMotorEventGroup, MASK(xTimerId));
    if (uxBits & (MASK(xTimerId) != MASK(xTimerId)))
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


void vErrorHandler(char *file, int line)
{
    /* Supress -Wunused-parameter */
    (void)file;
    (void)line;

    /* Use debug view to read variables */
    __BKPT(255);

    for (;;)
    {
    }
}

