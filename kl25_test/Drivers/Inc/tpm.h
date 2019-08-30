/**
 * tpm.h
 * Driver module for MKL25 TPM peripheral.
 */

#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"
#include "fsl_bitaccess.h"

/* User headers */
#include "defines.h"


/* Global function prototypes */
void TPM0_vInit(const uint16_t usPeriod);
void TPM1_vInit(void);
void TPM2_vInit(void);
void TPM2_vLoadCounter(uint8_t ucBytes);
void TPM2_vStart(void);
void TPM2_vStop(void);
