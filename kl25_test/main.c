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
    TaskHandle_t handle;
//    bool nak = false; /* Negative acknowledgement */
    
//    GPIO_Init();
//    SysTick_Init();
//    RF_Init();
    
    if (xTaskCreate(CrcTask, (const char *)"CrcTask", CRCTASKSIZE / sizeof(portSTACK_TYPE), 0, CRCTASKPRIORITY, &handle) != pdPASS)
    {
        __BKPT();
    }
    
    if (xTaskCreate(AnalogTask, (const char *)"AnalogTask", ANALOGTASKSIZE / sizeof(portSTACK_TYPE), 0, ANALOGTASKPRIORITY, &handle) != pdPASS)
    {
        __BKPT();
    }
    
    if (xTaskCreate(CommTask, (const char *)"CommTask", COMMTASKSIZE / sizeof(portSTACK_TYPE), 0, COMMTASKPRIORITY, &handle) != pdPASS)
    {
        __BKPT();
    }
    
    vTaskStartScheduler();
    
    //    while (1)
    //    {
    //        
    //        while (!nak)
    //        {
    //            /* Transmit the frame */
    //            RF_SetTransmissionMode();
    //            UART0_TransmitPolling(frame);
    //            RF_SetReceiverMode();
    //        
    //            while (!g_rxFlag)
    //            {
    //                ; /* Wait until message received, loop only for testing */
    //            }
    //        
    //            g_rxFlag = false;
    //            
    //            /* Retransmit frame if checksum doesn't match */
    //            if (!strncmp((const char *)g_rxData, "NAK", UART0_RX_BUFSIZ))
    //            {
    //                nak = true;
    //            }
    //        }
    //        
    //        nak = false;
    //        
    //        RF_SetPowerdownMode();
    //    }
    
    while(1)
    {
        ; /* Should not get here! */
    }
    
    return 0;
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

