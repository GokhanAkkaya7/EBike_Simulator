/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : conf_project_settings.h
 * Author       : Gökhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Environment  : Microsoft Visual Studio 2022, Version 17.5.3.
 * Description  : Configuration file of project.
 * Project      : VESTEL Renesas Microsoft Layer.
 * Creation Date: 29.05.25
 ******************************************************************************************/

#pragma once

 /*------------------------------------ Includes ------------------------------------------*/

#include "tx_api.h"	
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

// Project Settings.
#define SIMULATOR_MODE					    1

// Drivers.
#define ADC_FW                              1
#define CANBUS_DRV                          1
#define IRQ_DRV                             1
#define GPT_DRV                             1
#define IO_DRV                              1

// APP Settings.
#define APP_TEST                            1
#define ADC_TEST                            1
#define CANBUS_TEST                         1
#define IRQ_TEST                            1
#define GPT_TEST                            1
#define IO_TEST                             1



/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
    APP_SUCCESS = 0,

    APP_ERR_ASSERTION = 1,              // Given parameters are not appropriate.
    APP_ERR_UART_WRITE = 2,             // UART write failed.
    APP_ERR_UART_READ = 3,              // UART read failed
    APP_ERR_DRV_CONFIGURE = 4,          // Driver is not configured before.
    APP_ERR_THREADX = 5,                // Any ThreadX related fail.
    APP_ERR_THREADX_TIMEOUT = 6,        // ThreadX operation timeout happened.
    APP_ERR_UART_WRITE_TIMEOUT = 7,     // UART write operation timeout happened.
    APP_ERR_FSP_ERR = 8,                // An unexpected error happened on FSP layer.
    APP_ERR_UART_RX_EN_PIN_ERROR = 9,   // RX_EN_PIN can't be controlled on UART write.
    APP_ERR_VEE_WRITE_TIEMOUT = 10,     // VEE can't write to flash in a timely manner
    APP_ERR_VEE_RECORD_NOT_FOUND = 11,  // There is no record with the given ID.
    APP_ERR_MX25_BUSY = 12,             // Couldn't complete MX25 operation it keeps busy.
    APP_ERR_CAN_WRITE_TIMEOUT = 13,     // CAN write operation timeout happened.
    APP_ERR_CLOCK_STATUS_ERROR = 14,    // Clock current status can't be read.
    APP_ERR_CLOCK_START_ERROR = 15,     // Clock can't be started.
    APP_ERR_CLOCK_INIT_ERROR = 16,      // Clock can't be initialised.
    APP_ERR_CLOCK_INACTIVE = 17,        // Clock is not active.
    APP_ERR_AFE_READ_PARSER_ERROR = 18,
    APP_ERR_AFE_READ_TIMEOUT_ERROR = 19,
    APP_ERR_NO_VALID_DATA = 20,
    APP_ERR_IIC_WRITE_TIMEOUT = 21,
    APP_ERR_IIC_READ_TIMEOUT = 22,
    APP_ERR_BQ_FAULT_MASK_MISMATCH = 23,
    APP_ERR_BQ_INVALID_FAULT_RESET_TYPE = 24,

    APP_AFE_READ_WAITING = 25,
    APP_FAIL = 26,
    APP_SPI_EVENT_TIMEOUT = 27,

    APP_SHT4XA_READ_WAITING = 30,
    APP_SHT4XA_READ_SUCCES = 31,
    APP_ERR_SHT4XA_READ_FAIL = 32,
    APP_ERR_SHT4XA_WRITE_FAIL = 33, //TODO @ACC: Name changed since humidity not implemented in app there is no error when it added change them

    APP_DEBUG_LOG_STRUCT_FULL = 35,
    APP_DEBUG_LOG_NO_SUCH_LOG = 36,

    APP_ERR_NOT_STABILIZED = 37,
    APP_ERR_SSP_ERR = 38,
    APP_CAN_MODE_NOT_NORMAL = 39

} app_err_t;

/*----------------------------- Public Function Prototypes -------------------------------*/