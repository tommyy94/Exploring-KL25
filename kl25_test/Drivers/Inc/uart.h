/**
 * uart.h
 * Driver module for MKL25 UART peripheral.
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
#include "defines.h"

/* Global defines */
#define UART0_TX_PIN            (20UL)
#define UART0_RX_PIN            (21UL)
#define UART0_RX_BUFSIZ         (3UL)

/* Global variables */
extern uint8_t ucRxData[UART0_RX_BUFSIZ];
extern uint8_t ucRxFlag;

/* Global function prototypes */
void UART0_vInit(const uint32_t ulBaudrate);
uint32_t UART0_ulReadPolling(void);
void UART0_vTransmitByte(const char ucByte);
void UART0_vTransmitPolling(const char *pcData);
