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
#include "stdio.h"
#include "time.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

// Threadx Settings
#define app_unit_ms						    (1/ (1000/TX_TIMER_TICKS_PER_SECOND))
#define MS_IN_SEC							(1000 * app_unit_ms)
#define MS_IN_MIN							(60 * MS_IN_SEC)
#define MS_IN_HOUR							(60 * MS_IN_MIN)
#define MS_IN_DAY							(24 * MS_IN_HOUR)

// Project Settings.
#define SIMULATOR_MODE					    1
#define HERACLES_V2							1

// Drivers.
#define ADC_FW                              1
#define CANBUS_DRV                          1
#define IRQ_DRV                             1
#define GPT_DRV                             1
#define IO_DRV                              1
#define FLASH_DRV                           1
#define MX25_SPI_DRV                        1
#define RTC_DRV                             1
#define BQ78350								1
#define DBC_APP								1
#define EVENT_PUBSUB						1
#define CRC									1
#define RINGBUFFER							1
#define BQ32000								1
#define MONITOR_FW							1

#define MONITOR_FW                          1
#define SMBUS_FW                            1
#define IIC_FW                              1
#define MURATA_ZY                           1
#define DBC_APP                             1
#define COMMON_LOGGER                       1
#define CRC                                 1
#define EVENT_PUBSUB                        1
#define STATE_DISPLAY                       1
#define BQ32000                             1
#define AES                                 0

// APP Settings.
#define APP_TEST                            1
#define ADC_TEST                            1
#define CANBUS_TEST                         1
#define IRQ_TEST                            1
#define GPT_TEST                            1
#define IO_TEST                             1
#define CFLASH_TEST                         0
#define DFLASH_TEST                         1
#define SPI_TEST                            1
#define RTC_TEST                            1

// Log Config
#define VESTEL_LOG                          0
#define LOG_CAN_INTERFACE                   0
#define CAN_LOG_STREAM                      1

// Debug Flags.
#define LED_TEST                            0
#define DEBUG                               0
#define DEBUG_UART                          0
#define DEBUG_CANBUS                        0
#define DEBUG_SMBUS                         0
#define DEBUG_TIME                          0
#define DEBUG_LOGGER                        0
#define DEBUG_GAUGE                         0
#define DEBUG_BRAIN                         0
#define DEBUG_SPI                           0
#define DEBUG_BLE                           0
#define DEBUG_OTA                           0
#define DEBUG_MONITOR                       0
#define DEBUG_INIT                          0
#define DEBUG_MRTZY                         0
#define DEBUG_RTC                           1
#define DEBUG_CHARGER                       1
#define DEBUG_COMMON_LOG                    1 // Used for enabling software debug logs to be printed in communication interface of choice
#define DEBUG_DUAL_BATTERY                  1 // Used for taking UART logs from the dual battery about current state, SOC and switch messages.
#define DEBUG_SOC_DUAL_SWITCH               0 // Used for simulating dual switch in dsg. Timer decreases a mock soc value
#define DEBUG_SOC_LEVEL                     0
#define DEBUG_BLE_REMOTE_CTRL               0 // Used for keeping ble closed at start and enabling it to be controlled via communication protocols

// Define Based Options
#define IF04                                // HW Code
//#define MIVICE_CAN_A68                      // Terminal Code // TODO GA: open it later.
#define ANALOG_DOCK_DETECT                  // Dock Detection Method
#define UDS
#define NO_BLE                              // TODO: Change BLE compiler switch to do that

// Charger Options (Warning:only 1 must be selected)
#define CHARGE_GAUGE_CTRL                   1
#define CHARGE_ADC_CTRL                     0

// APP Settings
#define ECHO_PINS                           0
#define DOCK_CONTROL                        1
#define SLEEP_AT_DOCK                       1
#define CAN_APP                             1
#define CAN_WAKE_UP                         1
#define BLE                                 1
#define BLE_PIN_ACTIVATION                  0
#define BLE_ANIM_ON                         1
#define RTC_PERIOD                          500 // 500msec. z  C C                                                                                GN
#define UNUSED_COMMANDS                     0
#define CELL_COUNT                          10
#define CELL_PARALLEL_COUNT                 3
#define IS_FET_OFF_DISABLED                 0
#define PROFILING                           0
#define OTA                                 1
#define BIOS_OTA                            1
#define SHUTDOWN_FOR_SHORTCIRCUIT           1
#define PREDISCHARGE_MODE                   1
#define PIN_BASED_CHARGER_DETECT            0
#define DSG_DURING_CHG                      1
#define EXTERNAL_RTC                        1
#define MANUAL_RTC_INIT                     1
#define SELF_CYCLE_COUNT                    1
#define PACK_VOLT_SHUT_CTRL                 1
#define RINGBUFFER                          1

#define SWVER                               "lindwurm_v1.8.2"           // Software version of mivice IF04 project.
#define HWVER                               "28BTS12-R3"                // Hardware version string for IF04 mivice can.
#define MODEL_NO                            "VstlAcMv2CIF04"            // Model number string.
#define SWVER_CAN                           {1, 8, 2, 0}                // Software version composed as : Major_feature/minor_feature/bugfix/
#define MIVICE_PROTOCOL_VER                 {'A',69}                    // Protocol version A63
#define BATTERY_MACHINE_HEADER              "Battery/Can/ykz_can_battery_machine.h"
#define BRAIN_THREAD_MAX_LOOP_VAL           15
#define GAUGE_THREAD_MAX_LOOP_VAL           15
#define BLE_THREAD_MAX_LOOP_VAL             225
#define LOGGER_THREAD_MAX_LOOP_VAL          200


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