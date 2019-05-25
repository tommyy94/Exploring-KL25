/**
 * esp8266.h
 * Header for ESP8266 module.
 */

#pragma once

/* System headers */
#include "string.h"

/* Device vendor headers */

/* User headers */
#include "comm.h"

/* Global defines */
#define AT              ("AT\r\n")
#define AT_RESET        ("AT+RST\r\n")

/* Global variables */

/* Global function prototypes */
void ESP8266_vInit(void);
void ESP8266_vSendCmd(const char *pcCmd);
