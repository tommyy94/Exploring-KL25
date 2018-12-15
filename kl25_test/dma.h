/**
 * gpioh.
 * This compilation unit handles Direct Memory Access.
 */

#pragma once

/* System ehaders */
#include <stdint.h>

/* Devices vendor headers */
#include "MKL25Z4.h"

/* User headers */
#include "defines.h"
#include "dma_multiplexer_ch.h"


void DMA0_Init(uint32_t *dstAddr);
