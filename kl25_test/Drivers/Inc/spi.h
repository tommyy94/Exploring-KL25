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

/* Global function prototypes */
void SPI1_vInit(void);
void SPI1_vTransmitByte(const char ucByte);
void SPI1_vTransmitPolling(const char *pcData);
