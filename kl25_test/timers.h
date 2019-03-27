/**
 * timers.h
 * This header declares all timer related functions and variables.
 */

#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"

/* Global variables */
volatile uint32_t g_sTicks; /* Store second ticks */

/* Global function prototypes */
void WDT_Init(void);
void SysTick_Init(void);
void Service_COP_WDT(void);
void DelayUs(uint32_t us);
