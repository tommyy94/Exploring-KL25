/* User headers */
#include  "HS1101.h"

/* Local defines */
#define HUMIDITY_FORMULA(x) ((x))

/* Local variables */
volatile uint32_t g_HS1101_value = 0;
volatile bool g_HS1101_flag = false;

/* Local function prototypes */

/* Function descriptions */
/**
 * This function simply sets ports output & writes high to memory 
 * so multiplexing pins sets pin high.
 */
void HS1101_Init(void)
{
    FGPIOE->PDDR |= MASK(HUMID_SENSOR_PIN);
    FGPIOE->PSOR |= MASK(HUMID_SENSOR_PIN);
}


/**
 * This function is written in assembly to minimize time between enabling CMP0
 * and starting TMP1 to get most accurate timer value possible.
 * 
 * TODO: Calibrate humidity sensor
 * TODO: Figure CPU register usage
 */
void HS1101_SendSignal(void)
{
    __ASM(
        /* Initialize hw registers first */
        /* Initialize registers to set pin as GPIO */
        "ldr    r0, =0x4004D000\n\t"                /* Load PORTE base address to register */
        "mov    r1, #0x80\n\t"                      /* Can load only 8-bit values */
        "lsl    r1, r1, #1\n\t"                     /* Shift left once to generate 16-bit value (ALT1) */
    
        /* Initialize registers for starting TPM1 */
        "ldr    r3, =0x40039000\n\t"                /* Load TPM1 base address to register */
        "ldr    r2, [r3]\n\t"
        "ldr    r3, =0x40039000\n\t"
        "mov    r4, #8\n\t"
        "orr    r2, r4\n\t"
    
        /* Initialize registers to set pin as CMP0 input */
        "ldr   r5, =0x4004D000\n\t"                 /* Load PORTE base address to register */
        "mov   r6, #0\n\t"                          /* Reset register by writing 0 */
        
        /* Do work here */
        /* Set pin as GPIO charging HS1101 */
        "str    r1, [r0, #0x74]\n\t"                /* Store result to peripheral address + offset 4 * 29 = 116 = 0x74 */
        
        /* Start timer */
        "str    r2, [r3]\n\t"                       /* Write value to peripheral address */
        
        /* Select CMP0 input for pin */
        "str   r6, [r5, #0x74]"                     /* Write value to peripheral address */
    );
}


uint32_t HS1101_ReadHumidity(void)
{
    uint32_t humid = 0;
    
    while (!g_HS1101_flag)
    {
        ; /* Wait until humidity can be read */
    }
    
    g_HS1101_flag = false;
    
    humid = HUMIDITY_FORMULA(g_HS1101_value);
    
    return (humid);
}
