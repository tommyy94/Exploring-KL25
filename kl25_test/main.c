#include "includes.h"


void Delay(volatile unsigned int time_del);


int main(void)
{
    uint16_t ADC_Result;
    
    
    GPIO_Init();
    ADC0_Init();
    UART0_Init(115200);
    DMA0_Init((uint32_t *)&ADC_Result);
    TPM0_Init();
    WDT_Init();
    SysTick_Init();
    
    TPM0_Start();
    
	while(1)
	{
//    	ADC_Result = ADC0_ReadPolling();
    	
        ADC0->SC1[0] = 0x00;
    	while (1);
    	
//    	UART0_TransmitPolling("abc\r\n");
//    	Service_COP_WDT();
//    	Delay(200000);
	}

	return 0;
}


void Delay(volatile unsigned int time_del)
{
    while (time_del--)
    {
        ;
    }
}
