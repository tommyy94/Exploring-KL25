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
#include "comm.h"
#include "gpio.h"
#include "esp8266.h"
#include "dma.h"


/* Global defines */
#define MSEC_TO_TICK(msec)  (((uint32_t)(msec)+500uL/(uint32_t)configTICK_RATE_HZ) \
                             *(uint32_t)configTICK_RATE_HZ/1000uL)
#define TICKS_TO_MSEC(tick) ((tick)*1000uL/(uint32_t)configTICK_RATE_HZ)
    
#define ANALOGTASKSIZE 1024
#define ANALOGTASKPRIORITY 3
    
#define CRCTASKSIZE 1024
#define CRCTASKPRIORITY 2
    
#define COMMTASKSIZE 4096
#define COMMTASKPRIORITY 1
    
#define MOTORTASKSIZE 1024
#define MOTORTASKPRIORITY 4

#define MAX_QUEUE_SIZE      (32UL)

    
/* Global variables */
extern QueueHandle_t xCommQueue;
extern QueueHandle_t xAnalogQueue;
extern QueueHandle_t xMotorQueue;
extern EventGroupHandle_t xMotorEventGroup;
extern EventGroupHandle_t xTimeoutEventGroup;
extern SemaphoreHandle_t xCommSemaphore;
    

/* Global function prototypes */
void vSystemInit(TimerHandle_t *const pxTimeoutTimers);
void vCreateQueues(void);
void vCreateEvents(void);
void vCreateTasks(void *const pvMotorTimers, void *const pvTimeoutTimers);
void vCreateMotorTimers(TimerHandle_t *const pxTimers);
void vCreateSemaphores(void);
void vMotorTimerCallback(const TimerHandle_t xTimer);
void vTimeoutTimerCallback(const TimerHandle_t xTimer);
void vAssertCalled(const uint32_t ulLine, char *const pcFile);
