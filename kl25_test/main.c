/**
 * Display uptime, soil moisture, humidity and temperature in LCD.
 * Send the data to database/web server.
 *
 * Sleep -> TPM_OVF (wkup) -> ADC0 trig'd -> UART0 data -> Sleep
 * 
 * TODO: Test CRC
 * TODO: Figure out WDT
 * TODO: Add LCD
 */
#include <includes.h>



int main(void)
{
    TaskHandle_t task_func_task_handler;
    
    char frame[MAX_FRAME_SIZE];
    char crc32Frame[MAX_FRAME_SIZE];
    uint32_t crc32 = 0;
    bool nak = false; /* Negative acknowledgement */
    
//    GPIO_Init();
    //SPI1_Init();
    //SysTick_Init();
//    RF_Init();
//        

    crcInit();
    
    if (xTaskCreate(AnalogTask, (const char *)"AnalogTask", ANALOGTASKSIZE / sizeof(portSTACK_TYPE), 0, ANALOGTASKPRIORITY, &task_func_task_handler) != pdPASS)
    {
        __BKPT();
    }
    
    if (xTaskCreate(CommTask, (const char *)"AnalogTask", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &task_func_task_handler) != pdPASS)
    {
        __BKPT();
    }
    
    vTaskStartScheduler();
    
    
    
    
    while (1)
    {   

        /* Build the frame with checksum */
        snprintf(frame, MAX_FRAME_SIZE, "abcdefgh0123456789");
        crc32 = crcFast((uint8_t *)frame, strlen(frame));
        snprintf(crc32Frame, MAX_FRAME_SIZE, "crc32:%x\004", (unsigned int)crc32);
        strncat(frame, crc32Frame, strlen(crc32Frame));
        
        while (!nak)
        {
            /* Transmit the frame */
            RF_SetTransmissionMode();
            UART0_TransmitPolling(frame);
            RF_SetReceiverMode();
        
            while (!g_rxFlag)
            {
                ; /* Wait until message received, loop only for testing */
            }
        
            g_rxFlag = false;
            
            /* Retransmit frame if checksum doesn't match */
            if (!strncmp((const char *)g_rxData, "NAK", UART0_RX_BUFSIZ))
            {
                nak = true;
            }
        }
        
        nak = false;
        
        RF_SetPowerdownMode();
    }
}


void _Error_Handler(char *file,
                    int line)
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

