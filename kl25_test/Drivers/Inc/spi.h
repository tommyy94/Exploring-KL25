/**
 * spi.h
 * Driver module for MKL25 SPI peripheral.
 */

#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"


void SPI1_vInit(void);
void SPI1_vTransmitPolling(const char *data);
