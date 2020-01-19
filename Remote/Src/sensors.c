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
            }
        }
        
        if (xMotorQueue != 0)
        {
            xAssert = xQueueSend(xMotorQueue, (void *)&pxMotors, (TickType_t)10);
            configASSERT(xAssert);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


/**
 * @brief   TPM1 IRQ Handler used to capture CMP0 output.
 * 
 * @param   None
 * 
 * @return  None
 * @todo    Convert capacitor value (HS1101_ulValue) to humidity.
 */
void TPM1_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulOverflows = 0;
    uint32_t HS1101_ulValue = 0;
    BaseType_t xAssert;

    if (BME_UBFX32(&TPM1->STATUS, TPM_STATUS_TOF_SHIFT, TPM_STATUS_TOF_WIDTH))
    {
        ulOverflows++;
    }

    if (BME_UBFX32(&TPM1->STATUS, TPM_STATUS_CH1F_SHIFT, TPM_STATUS_CH1F_WIDTH))
    {
        /* Overflows should not happen */
        configASSERT(ulOverflows == 0);

        /* Stop TPM1 */
        BME_AND32(&TPM1->SC, ~TPM_SC_CMOD(1));

        /* Read humidity */
        HS1101_ulValue = TPM1->CONTROLS[1].CnV;

        /* Conversion should have been in progress */
        configASSERT(xAnalogNotification != NULL);

        /* Notify task */
        xAssert = xTaskNotifyFromISR(xAnalogNotification, HS1101_ulValue, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        configASSERT(xAssert == pdPASS);

        /* No conversion in progress, so no tasks to notify */
        xAnalogNotification = NULL;

        /* Reset counters */
        TPM1->CNT = 0;
        ulOverflows = 0;
    }

    /* Reset all flags */
    BME_OR32(&TPM1->STATUS, TPM_STATUS_TOF(1) | TPM_STATUS_CH1F(1));
    BME_OR32(&TPM1->CONTROLS[1].CnSC, TPM_CnSC_CHF(1));

    /* Force context switch if xHigherPriorityTaskWoken is set to pdTRUE */
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
