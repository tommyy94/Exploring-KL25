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
#include "semphr.h"

/* User headers */
#include "defines.h"
#include "sensors.h"
#include "printf-stdarg.h"

/* Global defines */
#define MAX_FRAME_SIZE          (64UL)

/* Global variables */


/* Global function prototypes */
void vCommTask(void *const pvParam);
void vSqlTask(void *const pvpParam);
