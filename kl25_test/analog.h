/**
 * analog.h
 * This file handles all analog functionality.
 */
#pragma once

/* System includes */
#include <stdint.h>

/* System vendor headers */
#include "MKL25Z4.h"

/* User headers */


/* Global function prototypes */
void ADC0_Init(void);
uint16_t ADC0_ReadPolling(void);
