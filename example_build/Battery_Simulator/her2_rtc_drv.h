#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_rtc_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : RTC Driver Module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 25.03.25
 ******************************************************************************************/

#ifndef SRC_HER2_RTC_HER2_RTC_DRV_H_
#define SRC_HER2_RTC_HER2_RTC_DRV_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(RTC_DRV)
#include "conf_rtc_drv.h"
#include "driver_api.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
    rtc_div_256_sec = 6,
    rtc_div_128_sec,
    rtc_div_64_sec,
    rtc_div_32_sec,
    rtc_div_16_sec,
    rtc_div_8_sec,
    rtc_div_4_sec,
    rtc_div_2_sec,
    rtc_1_sec,
    rtc_2_sec
} rtc_period_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t rtc_drv_configure(void (*p_rtc_event)(void));
app_err_t rtc_drv_init(void);
app_err_t rtc_drv_start(void);
app_err_t rtc_drv_deinit(void);
app_err_t rtc_drv_set_irq(rtc_period_t period);
app_err_t rtc_drv_time_set(struct tm* p_time);
app_err_t rtc_drv_time_get(struct tm* t);
void RTC_DATAHANDLER(RtcData* in_rtc_data);

#endif
#endif
