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
#include <stdbool.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "task.h"

/* User headers */
#include "defines.h"

/* Global defines */
#define MAX_FRAME_SIZE          (64UL)
#define UART0_TX_PIN            (20UL)
#define UART0_RX_PIN            (21UL)
#define UART0_RX_BUFSIZ         (3UL)

/* Global variables */
extern uint8_t g_rxData[UART0_RX_BUFSIZ];
extern bool g_rxFlag;


/* Global function prototypes */
void UART0_Init(const uint32_t baudrate);
uint32_t UART0_ReadPolling(void);
void UART0_printf(const char *p_fmt, ...);
void UART0_TransmitPolling(const char *data);
void CommTask(void * const param);

