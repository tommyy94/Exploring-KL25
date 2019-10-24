/**
 * motor.h
 * This header declares all motor related functions and variables.
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
void vStartMotor(const uint32_t ulChannel, TimerHandle_t *const pxMotorTimers);
void vStopMotor(const uint32_t ulChannel, TimerHandle_t *const pxMotorTimers);
void vMotorTask(void *const pvMotorTimers);
