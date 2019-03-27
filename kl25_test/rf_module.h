/**
 * rf_module.h
 * This header handles Aurel Wireless TX-MID-3V radio module.
 * This module uses pins PTA0, PTA1, PTA2, PTA3.
 */
# pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"
#include "timers.h"
#include "comm.h"

/* Global variables */

/* Global function prototypes */
void RF_Init(void);
void RF_SetPowerdownMode(void);
void RF_SetIdleMode(void);
void RF_SetTransmissionMode(void);
void RF_SetReceiverMode(void);
