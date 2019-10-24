/**
 * spi.h
 * Driver module for MKL25 SPI peripheral.
 * 
 * SPI1 Populated IO:
 *  PTE1 - MISO
 *  PTE2 - SCK
 *  PTE3 - MOSI
 *  PTE4 - CS
 */

#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"
#include "string.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "dma.h"
#include "tpm.h"

/* Global defines */

/* Global function prototypes */
void SPI1_vInit(void);
uint8_t SPI1_ucReadPolling(void);
void SPI1_vTransmitByte(const char ucByte);
void SPI1_vTransmitPolling(char *const pucData, char *const pucRxData, const uint32_t ulLength);
void SPI1_vTransmitDMA(char const *pucTxData, char *const pucRxData, const uint32_t ulLength);
void SPI1_vSetSlave(const uint32_t ulState);
