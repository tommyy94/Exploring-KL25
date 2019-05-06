/**
 * timers.h
 * This header declares all timer related functions and variables.
 */

#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "HS1101.h"

/* Global defines */

/* Global variables */

/* Global function prototypes */
void TPM0_vInit(uint16_t usPeriod);
void TPM0_vStartPWM(uint8_t ucChannel);
void TPM0_vStopPWM(uint8_t channel);
void TPM1_vInit(void);
void vMotorTask(void *const param);
