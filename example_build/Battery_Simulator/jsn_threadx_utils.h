#pragma once
/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_threadx_utils.h
* Author       : Emre Erisgen
* Description  : Util functions related with ThreadX.
* Project      : VESTEL Common modules project
* Creation Date: 01.04.2021
******************************************************************************************/

#ifndef JSN_UTILS_JSN_THREADX_UTILS_H_
#define JSN_UTILS_JSN_THREADX_UTILS_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

bool activate_oneshot_timer(TX_TIMER* p_timer, ULONG timeout);
bool deactivate_oneshot_timer(TX_TIMER* p_timer);
bool change_periodic_timer(TX_TIMER* p_timer, ULONG timeout);

#endif
