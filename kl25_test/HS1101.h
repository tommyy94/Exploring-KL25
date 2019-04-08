#pragma once

/* User headers */
#include "analog.h"


/**
 * This function simply set ports output & writes high to memory 
 * so multiplexing pins sets pin high.
 */
__INLINE void HS1101_Init(void)
{
    FGPIOE->PDDR |= MASK(HUMID_SENSOR_PIN);
    FGPIOE->PSOR |= MASK(HUMID_SENSOR_PIN);
}


/**
 * This function is written in assembly to
 * minimize time between enabling CMP0 and starting TMP1.
 * TODO: Calibrate humidity sensor
 * TODO: Optimize this
 */
__INLINE void HS1101_SendSignal(void)
{
    //PORTE->PCR[HUMID_SENSOR_PIN] |= PORT_PCR_MUX(ALT1);
    /**
     * Select GPIO for pin setting pin high
     * generating start signal for HS1101
     */
    __ASM(
        "ldr    r3, =0x4004D000\n\t"    /* Load PORTE base address to register */
        "ldr    r2, [r3, #0x74]\n\t"    /* Load PCR address 4 * 29 = 116 = 0x74 to register */
        "ldr    r3, =0x4004D000\n\t"    /* Load PORTE base address to register again */
        "movs   r1, #0x80\n\t"
        "lsl    r1, r1, #1\n\t"
        "orr    r2, r1\n\t"
        "str    r2, [r3, #0x74]"
    );
    
    TPM1->SC |= TPM_SC_CMOD(1);
    /* Start TPM1 timer */
    __ASM(
        "ldr    R3, =0x40039000\n\t"
        "ldr    R2, [R3]\n\t"
        "ldr    R3, =0x40039000\n\t"
        "mov    R1, #8\n\t"
        "orr    R2, R1\n\t"
        "str    R2, [R3]"
    );
    
    //PORTE->PCR[HUMID_SENSOR_PIN] = PORT_PCR_MUX(ALT0);
    /* Select CMP0 for pin */
    __ASM(
        "ldr   R3, =0x4004D000\n\t"     /* Load PORTE base address to register */
        "mov   R2, #0\n\t"              /* Reset register */
        "str   R2, [R3, #0x74]"         /* Write value to peripheral address */
    );
}
