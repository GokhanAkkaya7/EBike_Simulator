#pragma once
/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_log.h
 * Author       : Gokhan Akkaya
 * Description  : Common logger module.
 * Project      : VESTEL Common modules project
 * Creation Date: 15.04.2022
 ******************************************************************************************/

#ifndef JSN_LOG_JSN_LOG_H_
#define JSN_LOG_JSN_LOG_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"
#if(COMMON_LOGGER)
#include "jsn_monitor_log.h"
#include "conf_log.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define LOG_IND_SIZE                        3
#define LOG_IND_SIZE_TIME                   9

#define LOG_IND_VERBOSE_LOC                 0      // it shows the location of the verbose pointer in the log array
#define LOG_IND_SIZE_LOC                    1      // it shows the location of the size indicator in the log array
#define LOG_IND_INDEX_LOC                   2      // it shows the location of the index indicator in the log array

#define CHECK_WRONG_FRAME_AREA(_x)          ((0 == _x) || (_x >= LOGGER_NUMBER))

// Common Logger Event Flags
#define FLASH_LOG_WRITE_EVENT				0x01		// writing flash logs via terminal event
#define FLASH_LOG_DELETE_EVENT				0x02		// delete flash logs via terminal event
#define FLASH_LOG_INFO_EVENT				0x04		// writing flash info via terminal event
#define FLASH_LOG_FULL_EVENT				0x08		// flash area full event
#define MONITOR_LOG_OPEN_EVENT              0x10        // monitor log open event
#define MONITOR_LOG_CLOSE_EVENT             0x20        // monitor log open event

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
    info, error, warning, debug, log_data_frame
} log_verbose_e;

typedef enum
{
    log_flash, log_monitor
} log_target_e;

/*----------------------------- Public Function Prototypes -------------------------------*/

bool log_init(void);
bool log_set_target(uint8_t log_area, log_target_e target, bool status);
void log_sw(uint8_t* text, uint32_t code, log_verbose_e verbose);
void log_frame(uint8_t log_area, uint8_t* arr, uint8_t data_len);
void log_write(uint8_t* p_log_str);

#endif
#endif
