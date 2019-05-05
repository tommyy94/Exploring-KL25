#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

#include "FreeRTOS.h"
#include "task.h"

/* User headers */
#include "analog.h"
#include "crc.h"
#include "comm.h"


/* Global defines */
#define MSEC_TO_TICK(msec)  (((uint32_t)(msec)+500uL/(uint32_t)configTICK_RATE_HZ) \
                             *(uint32_t)configTICK_RATE_HZ/1000uL)
#define TICKS_TO_MSEC(tick) ((tick)*1000uL/(uint32_t)configTICK_RATE_HZ)
    
#define ANALOGTASKSIZE 1024
#define ANALOGTASKPRIORITY 1
    
#define COMMTASKSIZE 1024
#define COMMTASKPRIORITY 2
    
#define CRCTASKSIZE 1024
#define CRCTASKPRIORITY 3

#define MAX_QUEUE_SIZE      (32UL)

    
/* Global variables */
extern QueueHandle_t xCommQueue;
extern QueueHandle_t xAnalogQueue;
    

/* Global function prototypes */
void vSystemInit();
void vCreateQueues(void);
void vCreateTasks(void);
void vErrorHandler(char *file, int line);
