#include "analog.h"


/* Local defines */
#define TEMP_SENSOR_PIN     (0UL) /* TMP36GT */
#define MOIST_SENSOR_PIN    (1UL) /* YL-69 */
#define HUMID_SENSOR_PIN    (2UL) /* HS1101 */


void ADC0_Init(void)
{
    /* Enable clock to ADC0 & PORTB */
    SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
    
    /* Select analog for pin */
    PORTB->PCR[TEMP_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    PORTB->PCR[MOIST_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    PORTB->PCR[HUMID_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    
    /**
     * Low power configuration
     * Long sample time - 24 ADCK cycles total
     * 16 bit single-ended conversion
     * Bus clock input
     */
    ADC0->CFG1 = ADC_CFG1_ADLPC(1) | ADC_CFG1_ADLSMP(1) | ADC_CFG1_MODE(3);
    
     /**
     * Software trigger
     * Compare function disabled
     * Default voltage reference (VREFH & VREFL)
     */
    ADC0->SC2 = ADC_SC2_REFSEL(0);
    
    /**
     * Enable hardware average
     * 32 samples averaged
     */
    ADC0->SC3 = ADC_SC3_AVGE(1) | ADC_SC3_AVGS(ADC_SC3_AVGS_32SAMPLES);
}

        
/* Read frequency 10 Hz */
uint16_t ADC0_ReadPolling(const uint8_t channel)
{
    /* Start conversion on selected channel */
    ADC0->SC1[0] = ADC_SC1_ADCH(channel);
    
    while (!(ADC0->SC1[0] & ADC_SC1_COCO(1)))
    {
        ; /* Wait until conversion is finished */
    }
    
    /* Read the result */
    return ((uint16_t)ADC0->R[0]);
}
