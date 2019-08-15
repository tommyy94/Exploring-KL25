#include "sensors.h"


/* Local defines */


/* Global variables */
QueueHandle_t xAnalogQueue;
QueueHandle_t xMotorQueue;


/* Function descriptions */


/**
 * @brief   FreeRTOS sensor task.
 * 
 * @param   pvParam     Unused.
 * 
 * @return  None
 */
void vSensorTask(void *const pvParam)
{
    (void)pvParam;
    BaseType_t xAssert;
    
    struct Sensor xSensor;
    struct Sensor *pxSensor = &xSensor;

    struct Motor_States xMotors;
    struct Motor_States *pxMotors = &xMotors;
    
    const uint8_t ucSoilMoistureChannels[] = {ADC_CH_AD9, ADC_CH_AD9}; /* Same channel to simulate multiple sensors */
    
    for (;;)
    {
        /* Read all sensor values */
        xSensor.ulPotentiometer = ADC0_usReadPolling(ADC_CH_AD12); /* Not printed */
        
        xSensor.ulHumidity = HS1101_ulReadHumidity();
        //configASSERT(xSensor.ulHumidity <= MAX_HUMIDITY); /* Calibrate sensor first */
        
        xSensor.lTemperature = CELSIUS_TEMPERATURE(ADC0_usReadPolling(ADC_CH_AD8));
        configASSERT(xSensor.lTemperature >= MIN_TEMPERATURE && (xSensor.lTemperature <= MAX_TEMPERATURE));
        
        for (uint8_t i = 0; i < SOIL_MOISTURE_SENSOR_COUNT; i++)
        {
            xSensor.ulSoilMoisture[i] = SOIL_MOISTURE(ADC0_usReadPolling(ucSoilMoistureChannels[i]));
            configASSERT(xSensor.ulSoilMoisture[i] <= MAX_SOIL_MOISTURE);
        }
        
        if (xAnalogQueue != 0)
        {
            xAssert = xQueueSend(xAnalogQueue, (void *)&pxSensor, (TickType_t)10);
            configASSERT(xAssert);
        }
        
        for (uint8_t i = 0; i < SOIL_MOISTURE_SENSOR_COUNT; i++)
        {
            if (xSensor.ulSoilMoisture[i] < SOIL_MOISTURE_THRESHOLD)
            {
                xMotors.ucMotorState[i] = TRUE;
                
                /* Compute motor run time here */
            }
        }
        
        if (xMotorQueue != 0)
        {
            xAssert = xQueueSend(xMotorQueue, (void *)&pxMotors, (TickType_t)10);
            configASSERT(xAssert);
        }
        
        vTaskDelay(MSEC_TO_TICK(500));
    }
}
