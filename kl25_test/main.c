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
    struct ADC_Values adc;
    uint32_t humid = 0;
    
    volatile uint32_t crc = 0;
    
    //GPIO_Init();
    ADC0_Init();
    UART0_Init(4800);
    //SPI1_Init();
    //WDT_Init();
    SysTick_Init();
    RF_Init();
    
    TPM1_Init();
    CMP0_Init();
    HS1101_Init();
    
    crcInit();
    
    while (1)
    {
        //crc = crcFast((uint8_t *)"abcd", 4);
        
        RF_SetTransmissionMode();
        
        HS1101_SendSignal();
        humid = HS1101_ReadHumidity();
        UART0_printf("Humidity: %d\r\n", humid);
        
        adc.CH8_Result = ADC0_ReadPolling(ADC_CH_AD8);
        UART0_printf("Temperature: %d\r\n", CELSIUS_TEMPERATURE(adc.CH8_Result));
        
        adc.CH9_Result = ADC0_ReadPolling(ADC_CH_AD9);
        UART0_printf("Soil moisture: %d\r\n", SOIL_MOISTURE(adc.CH9_Result));
        
        adc.CH12_Result = ADC0_ReadPolling(ADC_CH_AD12);
        UART0_printf("Potentiometer: %d\r\n", adc.CH12_Result);
        
        UART0_printf("Uptime: %d seconds\r\n", g_sTicks);
        
        RF_SetPowerdownMode();
        
        //        //Service_COP_WDT();
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
