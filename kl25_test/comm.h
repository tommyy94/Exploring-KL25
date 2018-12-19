/**
 * comm.h
 * This header handles all communication tasks. 
 */

#pragma once

/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */


/* Global function prototypes */
void UART0_Init(const uint32_t baudrate);
void UART0_TransmitPolling(const char *data);
void UART0_printf(const char *p_fmt, ...);
