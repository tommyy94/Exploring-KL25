/* User headers */
#include  "HS1101.h"

/* Local defines */
#define HUMIDITY_FORMULA(x) ((x))

/* Local variables */
TaskHandle_t xAnalogNotification = NULL;

/* Local function prototypes */
static void HS1101_vSendSignal(void);


/* Function descriptions */

/**
 * @brief   This function simply sets ports output & writes high to memory so
 * multiplexing pins sets pin high.
 * 
 * @param   None
 * 
 * @return  None
 */
void HS1101_vInit(void)
{
    FGPIOE->PDDR |= MASK(HUMID_SENSOR_PIN);
    FGPIOE->PSOR |= MASK(HUMID_SENSOR_PIN);
}


/**
 * @brief   Measure HS1101 capacitance using CMP0
 * and TMP0 Input Capture mode.
 * 
 * @param   None
 * 
 * @return  None
 * 
 * @todo    Calibrate humidity sensor
 */
static void HS1101_vSendSignal(void)
{
    PORTE->PCR[HUMID_SENSOR_PIN] |= PORT_PCR_MUX(ALT1);
    TPM1->SC |= TPM_SC_CMOD(1);
    PORTE->PCR[HUMID_SENSOR_PIN] = PORT_PCR_MUX(ALT0);
}


/**
 * @brief   Read variable capacitor HS1101 using CMP0 and TMP0.
 * 
 * @param   None
 * 
 * @return  ulHumid     Air humidity value.
 */
uint32_t HS1101_ulReadHumidity(void)
{
    BaseType_t xAssert;
    const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
    uint32_t HS1101_ulValue = 0;
    uint32_t ulHumid = 0;
    
    /* No conversion should be in progress */
    configASSERT(xAnalogNotification == NULL);
    
    xAnalogNotification = xTaskGetCurrentTaskHandle();
    
    /* Start conversion */
    HS1101_vSendSignal();
    
    /* Wait until conversion done */
    xAssert = xTaskNotifyWait(0x00, 0xFFFFFFFF, &HS1101_ulValue, xTicksToWait);
    configASSERT(xAssert == pdPASS);
    
    /* Convert analog value and return it */
    ulHumid = HUMIDITY_FORMULA(HS1101_ulValue);
    return (ulHumid);
}
