#include "system.h"


/* Global variables */
EventGroupHandle_t xMotorEventGroup;


/* Local defines */
#define TIMER_NAME_LEN      (32UL)


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
    DMA0_vInit();
    UART0_vInit(115200);
    CRC_vInit();
    ESP8266_vInit();
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
    configASSERT(xAnalogQueue);
    
    xCommQueue = xQueueCreate(MAX_QUEUE_SIZE, sizeof(char *));
    configASSERT(xCommQueue);
    
    xMotorQueue = xQueueCreate(MAX_QUEUE_SIZE, sizeof(char *));
    configASSERT(xMotorQueue);
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
    configASSERT(xMotorEventGroup);
}


/**
 * @brief   Create FreeRTOS tasks.
 * 
 * @param   pvParameters    FreeRTOS software timers for Motor Task.
 * 
 * @return  None
 */
void vCreateTasks(void *const pvParameters)
{
    TaskHandle_t xHandle;
    BaseType_t xAssert;
    
    configASSERT((uint32_t) pvParameters);
    
    xAssert = xTaskCreate(vCrcTask, (const char *)"CRC", CRCTASKSIZE / sizeof(portSTACK_TYPE), 0, CRCTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vSensorTask, (const char *)"Sensor", ANALOGTASKSIZE / sizeof(portSTACK_TYPE), 0, ANALOGTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vCommTask, (const char *)"Comm", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vMotorTask, (const char *)"Motor", MOTORTASKSIZE / sizeof(portSTACK_TYPE), pvParameters, MOTORTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
}


/**
 * @brief   Create FreeRTOS software timers. vTimerCallback is called after 100 ms has passed.
 * 
 * @param   pxTimers    Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
void vCreateTimers(TimerHandle_t *const pxTimers)
{
    int8_t cBytesWritten;
    char ucMotorTimerName[TIMER_NAME_LEN];
    
    for (uint32_t i = 0; i < MOTOR_COUNT; i++)
    {
        cBytesWritten = csnprintf(ucMotorTimerName, TIMER_NAME_LEN, "Motor Timer %u", i + 1);
        configASSERT(cBytesWritten >= 0);
        pxTimers[i] = xTimerCreate(ucMotorTimerName, pdMS_TO_TICKS(100), pdTRUE, (void *)i, vTimerCallback);
        configASSERT(pxTimers[i]);
    }
}


/**
 * @brief   Create FreeRTOS semaphores.
 * 
 * @param   None
 * 
 * @return  None
 */
void vCreateSemaphores(void)
{
    xCommSemaphore = xSemaphoreCreateMutex();
    configASSERT(xCommSemaphore != NULL);
}


/**
 * @brief   FreeRTOS software timer callback. Sets event bit for motor.
 * 
 * @param   xTimer  Handle to callee software timer.
 * 
 * @return  None
 */
void vTimerCallback(const TimerHandle_t xTimer)
{
    EventBits_t uxBits;
    const uint32_t xTimerId = (uint32_t)pvTimerGetTimerID(xTimer);
    
    uxBits = xEventGroupSetBits(xMotorEventGroup, MASK(xTimerId));
    
    /* Check if correct bit was set */
    configASSERT(uxBits & (MASK(xTimerId)));
    
    /* Check if false bits were set */
    configASSERT(uxBits <= MASK(MOTOR_COUNT));
}


/**
 * @brief   System error handler.
 * 
 * @param   file    File name where error occurred
 * @param   line    Line where error occurred
 * 
 * @return  None
 */
void vAssertCalled(const uint32_t ulLine, char *const pcFile)
{
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
    
    /* Supress -Wunused-parameter */
    (void)pcFile;
    (void)ulLine;

    taskENTER_CRITICAL();

    while (ulSetToNonZeroInDebuggerToContinue == 0)
    {
        ;  /* Use debug view to read variables */
    }
    
    taskEXIT_CRITICAL();
}
