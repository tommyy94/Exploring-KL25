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


void main(void)
{
    struct Sensor_Values sensor;
    char frame[MAX_FRAME_SIZE];
    char crc32Frame[MAX_FRAME_SIZE];
    uint32_t crc32 = 0;
    bool nak = false; /* Negative acknowledgement */
    
//    GPIO_Init();
//    ADC0_Init();
    UART0_Init(9600);
    //SPI1_Init();
    SysTick_Init();
    RF_Init();
//        
//    TPM1_Init();
//    CMP0_Init();
//    HS1101_Init();

    crcInit();
    
    while (1)
    {   
//        /* Read all sensor values */
//        sensor.humidity = HS1101_ReadHumidity();
//        sensor.temperature = CELSIUS_TEMPERATURE(ADC0_ReadPolling(ADC_CH_AD8));
//        sensor.soil_moisture = SOIL_MOISTURE(ADC0_ReadPolling(ADC_CH_AD9));
//        sensor.potentiometer = ADC0_ReadPolling(ADC_CH_AD12); /* Not printed */

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
        
        DelayUs(10000);
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
