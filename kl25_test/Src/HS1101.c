/* User headers */
#include  "HS1101.h"

/* Local defines */
#define HUMIDITY_FORMULA(x) ((x))

/* Local variables */
volatile uint32_t ulHS1101_value = 0;
volatile uint8_t ulHS1101_flag = FALSE;

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
    uint32_t ulHumid = 0;
    
    HS1101_vSendSignal();
    
    /* TODO: Add timeout */
    while (!ulHS1101_flag)
    {
        ; /* Wait until humidity can be read */
    }
    
    ulHS1101_flag = FALSE;
    
    ulHumid = HUMIDITY_FORMULA(ulHS1101_value);
    
    return (ulHumid);
}
