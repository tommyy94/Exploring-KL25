#pragma once

/* System headers */

/* Device vendor headers */
#include "MKL25Z4.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"

/* User headers */
#include "comm.h"
#include "adc.h"
#include "tpm.h"
#include "dma.h"
#include "nrf24l01.h"


/* Global defines */
#define MSEC_TO_TICK(msec)      (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#define TICKS_TO_MSEC(tick)     ((tick)*1000uL / (uint32_t)configTICK_RATE_HZ)
#define MAX_QUEUE_SIZE          (32UL)

/* Task priorities */
#define ANALOGTASKPRIORITY      (4UL)
#define FRAMETASKPRIORITY         (5UL)
#define COMMTASKPRIORITY        (7UL)
#define MOTORTASKPRIORITY       (6UL)
#define STARTUPTASKPRIORITY     (10UL)

/* Task sizes */
#define ANALOGTASKSIZE          (2048UL)    
#define FRAMETASKSIZE             (1024UL)    
#define COMMTASKSIZE            (2048UL)    
#define MOTORTASKSIZE           (1024UL)    
#define STARTUPTASKSIZE         (4096UL)

    
/* Global variables */
extern QueueHandle_t xCommQueue;
extern QueueHandle_t xAnalogQueue;
extern QueueHandle_t xMotorQueue;
extern EventGroupHandle_t xMotorEventGroup;
extern SemaphoreHandle_t xCommSemaphore;
    

/* Global function prototypes */
void vSystemInit(void);
void vCreateQueues(void);
void vCreateEvents(void);
void vCreateTasks(void *const pvMotorTimers);
void vCreateMotorTimers(TimerHandle_t *const pxTimers);
void vCreateTimeoutTimers(TimerHandle_t *const pxTimers);
void vCreateSemaphores(void);
void vMotorTimerCallback(const TimerHandle_t xTimer);
void vStartupTask(void *const pvMotorTimers);
void vApplicationIdleHook(void);
void vAssertCalled(const uint32_t ulLine, char *const pcFile);
