#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : conf_rtc_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Configuration file of RTC driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 25.03.25
 ******************************************************************************************/

#ifndef SRC_CONF_CONF_RTC_DRV_H_
#define SRC_CONF_CONF_RTC_DRV_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(RTC_DRV)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define RTC_CALLBACK						1

#define DEFAULT_SECOND                      10
#define DEFAULT_MINUTE                      0
#define DEFAULT_HOUR                        0
#define DEFAULT_WDAY                        0
#define DEFAULT_MDAY                        1
#define DEFAULT_MON                         0
#define DEFAULT_YEAR                        100

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif

#endif
