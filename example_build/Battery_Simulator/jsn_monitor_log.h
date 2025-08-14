#pragma once
/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_monitor_log.h
* Author       : Emre Erisgen
* Description  : Common logger monitor module.
* Project      : VESTEL Common modules project
* Creation Date: 18.01.2023
******************************************************************************************/

#ifndef LOG_JSN_MONITOR_LOG_H_
#define LOG_JSN_MONITOR_LOG_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(COMMON_LOGGER)
#include "conf_log.h"

#if(LOGGER_MONITOR)


/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define MONITOR_SEND_TIMEOUT				250     // 250msec this is send from log uart + wait after send period.

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

bool monitor_log_init(void);
void monitor_log_send(uint8_t* p_log_str);


#endif
#endif
#endif
