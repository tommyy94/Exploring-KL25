/**
 * HS1101.h
 * This header declares all HS1101 related functions.
 */
#pragma once

/* System headers */
#include <stdbool.h>
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"

/* Global defines */

/* Global variables */
extern volatile uint32_t g_HS1101_value;
extern volatile bool g_HS1101_flag;


void HS1101_Init(void);
uint32_t HS1101_ReadHumidity(void);
