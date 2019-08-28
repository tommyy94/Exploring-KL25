/**
 * nrf24l01.h
 * Driver for NRF24l01.
 */
#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "spi.h"

/* Global defines */

/* Global variables */


/* Function prototypes */
void nRF24L01_vInit(void);
uint8_t nRF24L01_ucReadRegister(const uint8_t ucRegister);
void nRF24L01_vWriteRegister(const uint8_t ucRegister, const uint8_t ucValue);
