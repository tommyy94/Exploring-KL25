/**
 * adc.c
 * Driver module for MKL25 ADC peripheral.
 */

#include "adc.h"


/* Local defines */
#define CMP0_OUT_PIN        (0UL)

/* Global variables */

/* Function descriptions */

/**
 * @brief   Initialize ADC0 peripheral.
 * 
 * @param   None
 * 
 * @return  None
 */
void ADC0_vInit(void)
{    
    /* Select analog for pin */
    PORTB->PCR[TEMP_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    PORTB->PCR[MOIST_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    PORTE->PCR[HUMID_SENSOR_PIN] |= PORT_PCR_MUX(ALT0);
    
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

        
/**
 * @brief   Read ADC0 by polling.
 * 
 * @param   None
 * 
 * @return  ADC0->R[0]      16-bit ADC0 bank A result.
 */
uint16_t ADC0_usReadPolling(const uint8_t ucChannel)
{
    /* Start conversion on selected channel */
    ADC0->SC1[0] = ADC_SC1_ADCH(ucChannel);
    
    while (!(ADC0->SC1[0] & ADC_SC1_COCO(1)))
    {
        ; /* Wait until conversion is finished */
    }
    
    /* Read the result */
    return ((uint16_t)ADC0->R[0]);
}


/**
 * @brief   Initialize CMP0 peripheral.
 * 
 * @param   None
 * 
 * @return  None
 */
void CMP0_vInit(void)
{
    /* Enable clock to comparator */
    SIM->SCGC4 |= SIM_SCGC4_CMP(1);
    
    /* Enable clock to PORTE */
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    /* Select pin multiplexer for CMP0 */
    PORTE->PCR[CMP0_OUT_PIN] |= PORT_PCR_MUX(ALT5); 
    
    /* Hysteresis level 1 */
    CMP0->CR0 = CMP_CR0_HYSTCTR(1);
    
    /**
     * Enable comparator
     * Enable output pin
     * Invert comparator output
     */
    CMP0->CR1 = CMP_CR1_EN(1) | CMP_CR1_OPE(1) | CMP_CR1_INV(1);
    
    /**
     * Select input channels
     * Plus: channel 5 on PORTE29
     * Minus: DAC is channel 7 (internally connected)
     */
    CMP0->MUXCR |= CMP_MUXCR_PSEL(5) | CMP_MUXCR_MSEL(7);
    
    /**
     * Enable 6-bit DAC
     * Set reference voltage at 0.3V => 64 * 0.3V / 3.3V = 6
     */
    CMP0->DACCR = CMP_DACCR_DACEN(1) | CMP_DACCR_VOSEL(6);
}
