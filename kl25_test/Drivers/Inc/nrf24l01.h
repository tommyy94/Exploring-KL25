/**
 * nrf24l01.h
 * Driver for NRF24l01.
 */
#pragma once

/* System headers */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "fsl_bitaccess.h"
#include "FreeRTOS.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "spi.h"
#include "tpm.h"

/* Global defines */

/* Global variables */


/* Function prototypes */
void nRF24L01_vInit(void);
void nRF24L01_vResetStatusFlags(uint8_t ucStatusMask);
uint8_t nRF24L01_ucReadRegister(const uint8_t ucRegister);
void nRF24L01_vWriteRegister(const uint8_t ucRegister, const uint8_t ucValue);
void nRF24L01_vSendCommand(const uint8_t ucCommand);
void nRF24L01_vWrite8BitAddressRegister(const uint8_t ucRegister, const uint8_t ucValue);
void nRF24L01_vWrite40BitAddressRegister(const uint8_t ucRegister, const uint8_t *pucValue);
void nRF24L01_vSendPayload(uint8_t *pucPayload);
