/**
 * analog.h
 * This file handles all analog functionality.
 */
#pragma once

/* System includes */
#include <stdint.h>

/* Device vendor headers */
#include "MKL25Z4.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* User headers */
#include "defines.h"
#include "system.h"
#include "motor.h"

/* Global defines */
#define SOIL_MOISTURE_THRESHOLD                 (30UL)


struct Sensor
{
    int32_t lTemperature;
    uint32_t ulHumidity;
    uint32_t ulSoilMoisture[SOIL_MOISTURE_SENSOR_COUNT];
    uint32_t ulPotentiometer;
};

extern QueueHandle_t xAnalogQueue;


/* Global function prototypes */
void vSensorTask(void *const pvParam);
