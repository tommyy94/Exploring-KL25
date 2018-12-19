#include "analog.h"


/* Local defines */
#define ADC_POS (20)


void ADC0_Init(void)
{
    /* Enable clock to ADC0 & PORTE */
    SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
    
    /* Select analog for pin */
    PORTE->PCR[ADC_POS] |= PORT_PCR_MUX(0);
    
    /**
     * Low power configuration
     * Long sample time - 24 ADCK cycles total
     * 16 bit single-ended conversion
     * Bus clock input
     */
    ADC0->CFG1 |= ADC_CFG1_ADLPC(1) | ADC_CFG1_ADLSMP(1) | ADC_CFG1_MODE(3);
    
//    /**
//     * Hardware trigger
//     * Compare function disabled
//     * DMA enabled
//     * Default voltage reference (VREFH & VREFL)
//     */
//    ADC0->SC2 = ADC_SC2_ADTRG(1) | ADC_SC2_DMAEN(1) | ADC_SC2_REFSEL(0);
    
//    /**
//     * Alternate trigger for ADC0
//     * Pre-trigger A
//     * TPM0 overflow as trigger source
//     */
//    SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN(1) | SIM_SOPT7_ADC0PRETRGSEL(0) | SIM_SOPT7_ADC0TRGSEL(SIM_SOPT7_ADC0TRGSEL_TMP0_OVF);
    
     /**
     * Software trigger
     * Compare function disabled
     * DMA enabled
     * Default voltage reference (VREFH & VREFL)
     */
    ADC0->SC2 = ADC_SC2_DMAEN(1) | ADC_SC2_REFSEL(0);
    
    /**
     * Enable hardware average
     * 32 samples averaged
     */
    ADC0->SC3 = ADC_SC3_AVGE(1) | ADC_SC3_AVGS(ADC_SC3_AVGS_32SAMPLES);
}


uint16_t ADC_ReadPolling(ADC_Type *adc)
{
    uint16_t ADC_Result;
    
    /* Start conversion on channel 0 */
    adc->SC1[0] = 0x00;
    
    while (!(adc->SC1[0] & ADC_SC1_COCO(1)))
    {
        ; /* Wait until conversion is finished */
    }
    
    /* Read the result */
    ADC_Result = (uint16_t)adc->R[0];
    return ADC_Result;
}
