/**
 * adc.h
 * Driver module for MKL25 ADC peripheral.
 */

#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"

/* Global defines */
/* ADC0 trigger sources */
#define SIM_SOPT7_ADC0TRGSEL_EXTRG_IN   	    (0x00)       	/* External trigger pin input (EXTRG_IN) */	
#define SIM_SOPT7_ADC0TRGSEL_CMP0_OUT   	    (0x01)       	/* CMP0 output */
#define SIM_SOPT7_ADC0TRGSEL_PIT_TRIG0   	    (0x04)       	/* PIT trigger 0 */
#define SIM_SOPT7_ADC0TRGSEL_PIT_TRIG1   	    (0x05)       	/* PIT trigger 1 */
#define SIM_SOPT7_ADC0TRGSEL_TMP0_OVF   	    (0x08)       	/* TPM0 overflow */
#define SIM_SOPT7_ADC0TRGSEL_TMP1_OVF   	    (0x09)       	/* TPM1 overflow */
#define SIM_SOPT7_ADC0TRGSEL_TMP2_OVF   	    (0x0A)       	/* TPM2 overflow */
#define SIM_SOPT7_ADC0TRGSEL_RTC_ALARM   	    (0x0C)       	/* RTC alarm */
#define SIM_SOPT7_ADC0TRGSEL_RTC_SECONDS  	    (0x0D)       	/* RTC seconds */
#define SIM_SOPT7_ADC0TRGSEL_LPTMR0_TRIG  	    (0x0E)       	/* LPTMR0 trigger */

/* ADC hardware average select */
#define ADC_SC3_AVGS_4SAMPLES                   (0x00)          /* 4 samples averaged */
#define ADC_SC3_AVGS_8SAMPLES                   (0x01)          /* 8 samples averaged */
#define ADC_SC3_AVGS_16SAMPLES                  (0x02)          /* 16 samples averaged */
#define ADC_SC3_AVGS_32SAMPLES                  (0x03)          /* 32 samples averaged */

enum ADC_Channels
{
    ADC_CH_DADP0,
    ADC_CH_DADP1,
    ADC_CH_DADP2,
    ADC_CH_DADP3,
    ADC_CH_AD4,
    ADC_CH_AD5,
    ADC_CH_AD6,
    ADC_CH_AD7,
    ADC_CH_AD8,
    ADC_CH_AD9,
    ADC_CH_AD10,
    ADC_CH_AD11,
    ADC_CH_AD12,
    ADC_CH_AD13,
    ADC_CH_AD14,
    ADC_CH_AD15,
    ADC_CH_AD16,
    ADC_CH_AD17,
    ADC_CH_AD18,
    ADC_CH_AD19,
    ADC_CH_AD20,
    ADC_CH_AD21,
    ADC_CH_AD22,
    ADC_CH_AD23,
    ADC_CH_RESERVED0,
    ADC_CH_RESERVED1,
    ADC_CH_TEMP_SENSOR,
    ADCH_CH_BANDGAP,
    ADC_CH_VREFSH,
    ADC_CH_VREFSL,
    ADC_CH_DISABLED
};


/* Global function prototypes */
void ADC0_vInit(void);
uint16_t ADC0_usReadPolling(const uint8_t ucChannel);
void CMP0_vInit(void);
