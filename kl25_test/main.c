#include "includes.h"


void main(void)
{
    uint16_t ADC_Result;
    
    GPIO_Init();
    ADC0_Init();
    UART0_Init(115200);
    DMA0_Init((uint32_t *)&ADC_Result);
    TPM0_Init();
    WDT_Init();
    SysTick_Init();
    
	while(1)
	{
	    /* Start conversion on channel 0 */
	    ADC0->SC1[0] = 0x00;
    	
    	UART0_printf("ADC result: %d\r\n", ADC_Result);
    	Service_COP_WDT();
	}
}
