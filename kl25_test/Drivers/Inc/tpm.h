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

/* Timings calculated from 24 MHz clock speed */
#define MICROSECOND                         (24UL)                                      /* 1.0 탎 */
#define TEN_MICROSECONDS                    (MICROSECOND * 10)                          /* 10.0 탎 */
#define TIME_BETWEEN_BYTES                  (6UL)                                       /* 0.25 탎 */
#define TIME_PER_BYTE                       (MICROSECOND * 2 + TIME_BETWEEN_BYTES)      /* 2.25 탎 */

/* Global function prototypes */
void TPM0_vInit(const uint16_t usPeriod);
void TPM1_vInit(void);
void TPM2_vInit(void);
void TPM2_vLoadCounter(uint32_t ulBytes);
void TPM2_vStart(void);
void TPM2_vStop(void);
