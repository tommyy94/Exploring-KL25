/**
 * comm.h
 * This header handles all communication tasks. 
 */

#pragma once

/* System headers */
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* User headers */
#include "defines.h"
#include "analog.h"
#include "crc.h"
#include "printf-stdarg.h"

/* Global defines */
#define MAX_FRAME_SIZE          (64UL)
#define UART0_TX_PIN            (20UL)
#define UART0_RX_PIN            (21UL)
#define UART0_RX_BUFSIZ         (3UL)

/* Global variables */
extern uint8_t ucRxData[UART0_RX_BUFSIZ];
uint8_t ucRxFlag;


/* Global function prototypes */
void UART0_Init(const uint32_t baudrate);
uint32_t UART0_ReadPolling(void);
void UART0_TransmitByte(const char byte);
void UART0_TransmitPolling(const char *data);
void vCommTask(void *const param);
void vCrcTask(void *const param);
