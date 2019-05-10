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
 * @brief   This function is written in assembly to minimize time between sending
 * start signal and starting TMP1 to get most accurate timer value as possible.
 * 
 * @param   None
 * 
 * @return  None
 * 
 * @todo    Calibrate humidity sensor
 * @todo    Figure CPU register usage
 * @todo    Figure out parameters to inline assembly
 * @todo    Fix this shit
 */
static void HS1101_vSendSignal(void)
{
//    __ASM(
//        /* Initialize hw registers first */
//        /* Initialize registers to set pin as GPIO */
//        "ldr    r0, =0x4004D000\n\t"                /* Load PORTE base address to register */
//        "mov    r1, #0x80\n\t"                      /* Can load only 8-bit values */
//        "lsl    r1, r1, #1\n\t"                     /* Shift left once to generate 16-bit value (ALT1) */
//    
//        /* Initialize registers for starting TPM1 */
//        "ldr    r3, =0x40039000\n\t"                /* Load TPM1 base address to register */
//        "ldr    r2, [r3]\n\t"
//        "ldr    r3, =0x40039000\n\t"
//        "mov    r4, #8\n\t"
//        "orr    r2, r4\n\t"
//    
//        /* Initialize registers to set pin as CMP0 input */
//        "ldr   r5, =0x4004D000\n\t"                 /* Load PORTE base address to register */
//        "mov   r6, #0\n\t"                          /* Reset register by writing 0 */
//        
//        /* Do work here */
//        /* Set pin as GPIO charging HS1101 */
//        "str    r1, [r0, #0x74]\n\t"                /* Store result to peripheral address + offset 4 * 29 = 116 = 0x74 */
//        
//        /* Start timer */
//        "str    r2, [r3]\n\t"                       /* Write value to peripheral address */
//        
//        /* Select CMP0 input for pin */
//        "str   r6, [r5, #0x74]"                     /* Write value to peripheral address */
//    );
    
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
