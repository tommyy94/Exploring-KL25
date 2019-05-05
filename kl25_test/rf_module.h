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
#include "hw_timers.h"
#include "comm.h"

/* Global variables */

/* Global function prototypes */
void RF_vInit(void);
void RF_vSetPowerdownMode(void);
void RF_vSetIdleMode(void);
void RF_vSetTransmissionMode(void);
void RF_vSetReceiverMode(void);
