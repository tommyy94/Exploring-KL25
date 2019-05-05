#include "system.h"


void vSystemInit(void)
{
    /* Analog functionalities */
    ADC0_vInit();
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


void vCreateTasks(void)
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
    
    if (xTaskCreate(vCommTask, (const char *)"Communication", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &xHandle) != pdPASS)
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

    while (1)
    {
    }
}

