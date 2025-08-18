#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_monitor_fw.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Monitor Framework Module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 28.03.25
 ******************************************************************************************/

#ifndef SRC_HER2_MONITOR_HER2_MONITOR_FW_H_
#define SRC_HER2_MONITOR_HER2_MONITOR_FW_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(MONITOR_FW)
#include "conf_monitor_fw.h"
#include "jsn_utils.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef struct
{
    uint32_t minimum_count;
    uint32_t maximum_count;
} thread_monitor_min_max;

/*----------------------------- Public Function Prototypes -------------------------------*/

#if WDT_CALLBACK_ENABLED
void wdt_callback_configure(void (*p_wdt_callback)(void));
#endif

app_err_t monitor_fw_configure(void);
app_err_t monitor_fw_get_counts(thread_monitor_min_max* min_max_values);
app_err_t monitor_fw_thread_register(thread_monitor_min_max* min_max_values);
app_err_t monitor_fw_thread_unregister(void);
app_err_t monitor_fw_count_increment(void);

#endif
#endif
