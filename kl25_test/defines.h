/**
 * defines.h
 * This header declares global defines.
 */

#pragma once

#define SIGNAL_SHIFT (5)
#define MASK(pin) (1UL << (pin))

/* Sensor pins */
#define TEMP_SENSOR_PIN     (0UL)   /* TMP36GT */
#define MOIST_SENSOR_PIN    (1UL)   /* SEN0193 */
#define HUMID_SENSOR_PIN    (29UL)  /* HS1101 */

/* Sensor formulas */

/**
 * multiplier = 100
 * calibratedSteps = 51000 - 25000
 * moisture = (ADC * multiplier) / calibratedSteps
 * Sensor value is inverted, fix by subtracting ADC from 100
 */
#define SOIL_MOISTURE(adc)          (100 - ((adc) - 25000) * 100 / (51000 - 25000))


/**
 * steps = 2^16 = 0xFFFF
 * Vref = 3300 mV
 * mVout = (Vref / steps) * ADC
 * scaleFactor = 10
 * offsetVoltage = 500 mV
 * TempC = (mVout - offsetVoltage) / scaleFactor
 * Formula is optimized to avoid floating point calculation
 */
#define CELSIUS_TEMPERATURE(adc)    ((((3300 * (adc)) / 0xFFFF) - 500) / 10)


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
