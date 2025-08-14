#pragma once
/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_bq32000.h
* Author       : Gokhan Akkaya
* Description  : TI BQ32000 battery backup external RTC module.
* Project      : VESTEL Common modules project
* Creation Date: 02.04.2021
******************************************************************************************/

#ifndef JSN_EXRTC_JSN_BQ32000_H_
#define JSN_EXRTC_JSN_BQ32000_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"
#if(BQ32000)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

bool bq32000_init(void);
bool bq32000_time_set(const struct tm* p_time, uint8_t rtry_count);
bool bq32000_time_get(struct tm* p_time, uint8_t rtry_count);

#endif
#endif
