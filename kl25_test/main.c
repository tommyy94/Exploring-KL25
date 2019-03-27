/**
 * Display uptime, soil moisture, humidity and temperature in LCD.
 * Send the data to database/web server.
 *
 * Sleep -> TPM_OVF (wkup) -> ADC0 trig'd -> UART0 data -> Sleep
 * 
 * TODO: Write CRC
 * TODO: Figure out WDT
 */

#include <includes.h>


void main(void)
{
    struct ADC_Values adc;
    
    GPIO_Init();
    ADC0_Init();
    UART0_Init(9600);
    //SPI1_Init();
    //WDT_Init();
    SysTick_Init();
    RF_Init();
    
    
    while (1)
    {
        RF_SetTransmissionMode();
        UART0_printf("ABCD");
        RF_SetReceiverMode();
        RF_SetIdleMode();
        RF_SetPowerdownMode();
        RF_SetIdleMode();
        RF_SetTransmissionMode();
        RF_SetReceiverMode();
        RF_SetIdleMode();
        RF_SetTransmissionMode();
        RF_SetIdleMode();
        RF_SetReceiverMode();
        DelayUs(1000000);

//        /* Read temperature */
//        adc.CH8_Result = ADC0_ReadPolling(ADC_CH_AD8);
//        volatile uint16_t temp = CELSIUS_TEMPERATURE(adc.CH8_Result);
//        UART0_printf("Temperature: %d\r\n", CELSIUS_TEMPERATURE(adc.CH8_Result));
//        /* Read soil moisture */
//        adc.CH9_Result = ADC0_ReadPolling(ADC_CH_AD9);
//        UART0_printf("Soil moisture: %d\r\n", SOIL_MOISTURE(adc.CH9_Result));
        /* Read humidity */
//        adc.CH12_Result = ADC0_ReadPolling(ADC_CH_AD12);
//        UART0_printf("Humidity: %d\r\n", RELATIVE_HUMIDITY(adc.CH12_Result));
//        
//        UART0_printf("Uptime: %d seconds\r\n", g_sTicks);
        //Service_COP_WDT();
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
