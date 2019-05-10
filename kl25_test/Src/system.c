#include "system.h"


/* Global variables */
EventGroupHandle_t xMotorEventGroup;


/**
 * @brief   Initialize system hardware.
 * 
 * @param   None
 * 
 * @return  None
 */
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


/**
 * @brief   Create FreeRTOS queues.
 * 
 * @param   None
 * 
 * @return  None
 */
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
    
    xMotorQueue = xQueueCreate(MAX_QUEUE_SIZE, sizeof(char *));
    if (xMotorQueue == NULL)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


/**
 * @brief   Create 8-bit FreeRTOS Event Groups.
 * 
 * @param   None
 * 
 * @return  None
 */
void vCreateEvents(void)
{
    xMotorEventGroup = xEventGroupCreate();
    if (xMotorEventGroup == NULL)
    {
        vErrorHandler(__FILE__, __LINE__);
    }
}


/**
 * @brief   Create FreeRTOS tasks.
 * 
 * @param   pvParameters    FreeRTOS software timers for Motor Task.
 * 
 * @return  None
 */
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


/**
 * @brief   Create FreeRTOS software timers.
 * 
 * @param   pxTimers    Pointer to FreeRTOS software timers.
 * 
 * @return  None
 * @todo    Format timer names
 */
void vCreateTimers(TimerHandle_t *pxTimers)
{
    for (uint32_t i = 0; i < MOTOR_COUNT; i++)
    {
        pxTimers[i] = xTimerCreate("Motor Timer", pdMS_TO_TICKS(100), pdTRUE, (void *)i, vTimerCallback);
        if (pxTimers[i] == NULL)
        {
            vErrorHandler(__FILE__, __LINE__);
        }
    }
}


/**
 * @brief   FreeRTOS software timer callback.
 * 
 * @param   xTimer  Handle to callee software timer.
 * 
 * @return  None
 */
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


/**
 * @brief   System error handler.
 * 
 * @param   file    File name where error occurred
 * @param   line    Line where error occurred
 * 
 * @return  None
 */
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
