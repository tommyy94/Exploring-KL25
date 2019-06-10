/**
 * timers.h
 * This header declares all timer related functions and variables.
 */

#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "fsl_bitaccess.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "HS1101.h"

/* Global defines */

/* Global variables */
struct Motor_States
{
    uint8_t ucMotorState[MOTOR_COUNT];
    uint8_t ucMotorRunTime[MOTOR_COUNT];
};

/* Global function prototypes */
void TPM0_vInit(const uint16_t usPeriod);
void TPM0_vStartPWM(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers);
void TPM0_vStopPWM(const uint8_t ucChannel, TimerHandle_t *const pxMotorTimers);
void TPM1_vInit(void);
void vMotorTask(void *const pvMotorTimers);
