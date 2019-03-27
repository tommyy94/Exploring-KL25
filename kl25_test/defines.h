/**
 * defines.h
 * This header declares global defines.
 */

#pragma once

#define SIGNAL_SHIFT (5)
#define MASK(pin) (1UL << (pin))

/* Sensor formulas */
#define RELATIVE_HUMIDITY(adc)      ((adc) * 100 / 65536) /* Vout * 100% / 2^16 */
#define SOIL_MOISTURE(adc)          ((adc) * 100 / 65536) /* Vout * 100% / 2^16 */
#define CELSIUS_TEMPERATURE(adc)    (((adc) * 3.3 - 0.5) * 100)


/* Define alternative functions for pins. */
enum AlternativeFunctions
{
    ALT0,
    ALT1,
    ALT2,
    ALT3,
    ALT4,
    ALT5,
    ALT6,
    ALT7,
    RESET
};

void _Error_Handler(char *file, int line);
