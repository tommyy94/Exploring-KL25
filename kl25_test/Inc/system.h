#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"

/* User headers */
#include "defines.h"
#include "analog.h"
#include "crc.h"
#include "comm.h"
#include "gpio.h"


/* Global defines */
#define MSEC_TO_TICK(msec)  (((uint32_t)(msec)+500uL/(uint32_t)configTICK_RATE_HZ) \
                             *(uint32_t)configTICK_RATE_HZ/1000uL)
#define TICKS_TO_MSEC(tick) ((tick)*1000uL/(uint32_t)configTICK_RATE_HZ)
    
#define ANALOGTASKSIZE 1024
#define ANALOGTASKPRIORITY 2
    
#define CRCTASKSIZE 1024
#define CRCTASKPRIORITY 3
    
#define COMMTASKSIZE 1024
#define COMMTASKPRIORITY 4
    
#define MOTORTASKSIZE 1024
#define MOTORTASKPRIORITY 1

#define MAX_QUEUE_SIZE      (32UL)

    
/* Global variables */
extern QueueHandle_t xCommQueue;
extern QueueHandle_t xAnalogQueue;
extern QueueHandle_t xMotorQueue;
extern EventGroupHandle_t xMotorEventGroup;
    

/* Global function prototypes */
void vSystemInit();
void vCreateQueues(void);
void vCreateEvents(void);
void vCreateTasks(void *pvParameters);
void vCreateTimers(TimerHandle_t *pxTimers);
void vErrorHandler(char *file, int line);
void vTimerCallback(TimerHandle_t xTimer);
