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
#define AT                  ("AT\r\n")
#define AT_RESET            ("AT+RST\r\n")
#define AT_STA_MODE         ("AT+CWMODE=1\r\n")
#define AT_AP_MODE          ("AT+CWMODE=2\r\n")
#define AT_CONNECT_AP       ("AT+CWJAP_DEF=\"ssid\",\"pw\"\r\n")
#define AT_DISCONNECT_AP    ("AT+CWQAP\r\n")


/* Global variables */

/* Global function prototypes */
void ESP8266_vInit(TimerHandle_t *const pxTimeoutTimers);
void ESP8266_vSendCmd(char *const pcCmd, TimerHandle_t *const pxTimeoutTimers);
