#include "system.h"


/* Global variables */
EventGroupHandle_t xMotorEventGroup;
EventGroupHandle_t xTimeoutEventGroup;


/* Local defines */
#define TIMER_NAME_LEN          (32UL)


/* Local function prototypes */
static void vSystemInit(void);
static void vEnableClockGating(void);
static void vCreateQueues(void);
static void vCreateEvents(void);
static void vCreateTasks(void *const pvMotorTimers);
static void vCreateMotorTimers(TimerHandle_t *const pxTimers);
static void vCreateSemaphores(void);
static void vMotorTimerCallback(const TimerHandle_t xTimer);


/**
 * @brief   Initialize system hardware.
 * 
 * @param   None
 * 
 * @return  None
 */
static void vSystemInit(void)
{
    /* Power up all necessary peripherals */
    vEnableClockGating();

    /* Analog functionalities */
    ADC0_vInit();
    TPM0_vInit(4800);
    TPM1_vInit();
    TPM2_vInit();
    CMP0_vInit();
    HS1101_vInit();
    
    /* Communications */
    DMA0_vInit();
    DMAMUX0_vInit(DMA_CHANNEL0, DMAMUX_CHCFG_SOURCE_SPI1_TX);
    DMAMUX0_vInit(DMA_CHANNEL1, DMAMUX_CHCFG_SOURCE_SPI1_RX);
    DMA0_vLinkChannel(DMA_CHANNEL0, DMA_CHANNEL1);
    SPI1_vInit();
    nRF24L01_vInit();
}


/**
 * @brief   Enable clock gating to modules. Accessing peripheral while disabled generates hard fault.
 * 
 * @param   None
 * 
 * @return  None
 */
static void vEnableClockGating(void)
{
    SIM->SCGC4 |= SIM_SCGC4_SPI1(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1) | SIM_SCGC5_PORTB(1) | SIM_SCGC5_PORTD(1) | SIM_SCGC5_PORTE(1);
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1) | SIM_SCGC6_TPM1(1) | SIM_SCGC6_TPM2(1) | SIM_SCGC6_ADC0(1) | SIM_SCGC6_DMAMUX(1);
    SIM->SCGC7 |= SIM_SCGC7_DMA(1);
}


/**
 * @brief   Create FreeRTOS queues.
 * 
 * @param   None
 * 
 * @return  None
 */
static void vCreateQueues(void)
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
static void vCreateEvents(void)
{
    xMotorEventGroup = xEventGroupCreate();
    configASSERT(xMotorEventGroup);
    
    xTimeoutEventGroup = xEventGroupCreate();
    configASSERT(xTimeoutEventGroup);
}


/**
 * @brief   Create FreeRTOS tasks.
 * 
 * @param   pvParameters    FreeRTOS software timers for Motor Task.
 * 
 * @return  None
 */
static void vCreateTasks(void *const pvMotorTimers)
{
    TaskHandle_t xHandle;
    BaseType_t xAssert;
    
    configASSERT((uint32_t) pvMotorTimers);
    
    xAssert = xTaskCreate(vFrameTask, (const char *)"SQL", FRAMETASKSIZE / sizeof(portSTACK_TYPE), 0, FRAMETASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vSensorTask, (const char *)"Sensor", ANALOGTASKSIZE / sizeof(portSTACK_TYPE), 0, ANALOGTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vCommTask, (const char *)"Comm", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
    
    xAssert = xTaskCreate(vMotorTask, (const char *)"Motor", MOTORTASKSIZE / sizeof(portSTACK_TYPE), pvMotorTimers, MOTORTASKPRIORITY, &xHandle);
    configASSERT(xAssert);
}


/**
 * @brief   Create FreeRTOS software timers for motors. vTimerCallback is called after 100 ms has passed.
 * 
 * @param   pxTimers    Pointer to FreeRTOS software timers.
 * 
 * @return  None
 */
static void vCreateMotorTimers(TimerHandle_t *const pxTimers)
{
    int32_t lBytesWritten;
    char ucMotorTimerName[TIMER_NAME_LEN];
    
    for (uint32_t i = 0; i < MOTOR_COUNT; i++)
    {
        lBytesWritten = csnprintf(ucMotorTimerName, TIMER_NAME_LEN, "Motor Timer %lu", i + 1);
        configASSERT(lBytesWritten >= 0);
        pxTimers[i] = xTimerCreate(ucMotorTimerName, pdMS_TO_TICKS(100), pdTRUE, (void *)i, vMotorTimerCallback);
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
static void vCreateSemaphores(void)
{
    xCommSemaphore = xSemaphoreCreateMutex();
    configASSERT(xCommSemaphore != NULL);
}


/**
 * @brief   FreeRTOS software motor timer callback. Sets event bit for motor.
 * 
 * @param   xTimer  Handle to callee software timer.
 * 
 * @return  None
 */
static void vMotorTimerCallback(const TimerHandle_t xTimer)
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
 * @brief   Initializes OS, core and peripherals. Deleted afterwards.
 * 
 * @param   pvMotorTimers   Handle to motor timers.
 * 
 * @return  None
 */
void vStartupTask(void *const pvMotorTimers)
{
    /* Initialize FreeRTOS components */
    vCreateQueues();
    vCreateSemaphores();
    vCreateEvents();
    vCreateMotorTimers(pvMotorTimers);
    
    /* Initialize hardware */
    vSystemInit();
    
    /* Create tasks */
    vCreateTasks(pvMotorTimers);
    
    /* Startup task no longer needed */
    vTaskDelete(NULL);
}


/**
 * @brief   Idle task hook.
 * 
 * @param   None
 * 
 * @return  None
 */
void vApplicationIdleHook(void)
{
    ; /* Do nothing */
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


/**
 * Hard Fault handler.
 */
void HardFault_Handler(void)
{
    __BKPT();
}
