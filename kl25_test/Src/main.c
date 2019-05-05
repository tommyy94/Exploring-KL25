/**
 * Display uptime, soil moisture, humidity and temperature in LCD.
 * Send the data to database/web server.
 */
#include <includes.h>


int main(void)
{
    vSystemInit();
    vCreateTasks();
    vCreateQueues();
    vTaskStartScheduler();
    
    while (1)
    {
        ; /* Should not get here! */
    }
    
    //    bool nak = false; /* Negative acknowledgement */
    
    //    while (1)
    //    {
    //        
    //        while (!nak)
    //        {
    //            /* Transmit the frame */
    //            RF_vSetTransmissionMode();
    //            UART0_vTransmitPolling(frame);
    //            RF_vSetReceiverMode();
    //        
    //            while (!ucRxFlag)
    //            {
    //                ; /* Wait until message received, loop only for testing */
    //            }
    //        
    //            ucRxFlag = FALSE;
    //            
    //            /* Retransmit frame if checksum doesn't match */
    //            if (!strncmp((const char *)ucRxFlag, "NAK", UART0_RX_BUFSIZ))
    //            {
    //                nak = TRUE;
    //            }
    //        }
    //        
    //        nak = FALSE;
    //        
    //        RF_vSetPowerdownMode();
    //    }
}
