/**
 * analog.h
 * This file handles all analog functionality.
 */
#pragma once

/* System includes */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */

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


/* Global function prototypes */
void ADC0_Init(void);
uint16_t ADC_ReadPolling(ADC_Type *adc);
