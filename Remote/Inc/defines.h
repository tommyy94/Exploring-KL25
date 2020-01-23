/**
 * defines.h
 * This header declares global defines.
 */

#pragma once


/* Bit shifting */
#define SIGNAL_SHIFT                    (5) /* For testing purposes */
#define MASK(pin)                       (1UL << (pin))

/* Boolean logic */
#define FALSE                           (0UL)
#define TRUE                            (!FALSE)

/* Signal edges */
#define LOW     (0UL)
#define HIGH    (1UL)

/* Sensor pins */
#define TEMP_SENSOR_PIN                 (0UL)   /* TMP36GT */
#define MOIST_SENSOR_PIN                (1UL)   /* SEN0193 */
#define HUMID_SENSOR_PIN                (29UL)  /* HS1101 */

/* IC Counts */
#define SOIL_MOISTURE_SENSOR_COUNT      (2UL)
#define MOTOR_COUNT                     (1UL)   /* Max 5 */

/* Sensor thresholds */
#define MIN_TEMPERATURE                 (-40L)
#define MAX_TEMPERATURE                 (125L)
#define MAX_HUMIDITY                    (100UL)
#define MAX_SOIL_MOISTURE               (100UL)

/* Sensor formulas */

/**
 * multiplier = 100
 * calibratedSteps = 51000 - 25000
 * moisture = (ADC * multiplier) / calibratedSteps
 * Sensor value is inverted, fix by subtracting ADC from 100
 */
#define SOIL_MOISTURE(adc)              (100 - ((adc) - 25000) * 100 / (51000 - 25000))


/**
 * steps = 2^16 = 0xFFFF
 * Vref = 3300 mV
 * mVout = (Vref / steps) * ADC
 * scaleFactor = 10
 * offsetVoltage = 500 mV
 * TempC = (mVout - offsetVoltage) / scaleFactor
 * Formula is optimized to avoid floating point calculation
 */
#define CELSIUS_TEMPERATURE(adc)        ((((3300 * (adc)) / 0xFFFF) - 500) / 10)


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
