#include "analog.h"


/* Local defines */
#define ADC_POS (20)


void ADC0_Init(void)
{
    /* Enable clock to ADC0 & PORTE */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Select analog for pin */
    PORTE->PCR[ADC_POS] |= PORT_PCR_MUX(0);
    
    /**
     * Low power configuration
     * Long sample time
     * 16 bit single-ended conversion
     * Bus clock input
     */
    ADC0->CFG1 |= ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADLPC_MASK;
    
    /**
     * Software trigger
     * Compare function disabled
     * DMA enabled
     * Default voltage reference (VREFH & VREFL)
     */
    ADC0->SC2 = ADC_SC2_DMAEN(1) | ADC_SC2_REFSEL(0);
}


uint16_t ADC0_ReadPolling(void)
{
    uint16_t ADC_Result;
    
    /* Start conversion on channel 0 */
    ADC0->SC1[0] = 0x00;
    
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
        ; /* Wait until conversion is finished */
    }
    
    /* Read the result */
    ADC_Result = (uint16_t)ADC0->R[0];
    return ADC_Result;
}
