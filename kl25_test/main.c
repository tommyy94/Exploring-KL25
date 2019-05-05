/**
 * Display uptime, soil moisture, humidity and temperature in LCD.
 * Send the data to database/web server.
 */
#include <includes.h>


int main(void)
{
    //    bool nak = false; /* Negative acknowledgement */
    
    vSystemInit();
    vCreateTasks();
    vCreateQueues();
    vTaskStartScheduler();
    
    while (1)
    {
        ; /* Should not get here! */
    }
    
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
}
