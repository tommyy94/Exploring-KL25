/**
 * HS1101.h
 * This header declares all HS1101 related functions.
 */
#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "task.h"

/* User headers */
#include "defines.h"
#include "system.h"

/* Global defines */

/* Global variables */
extern TaskHandle_t xAnalogNotification;


void HS1101_vInit(void);
uint32_t HS1101_ulReadHumidity(void);
