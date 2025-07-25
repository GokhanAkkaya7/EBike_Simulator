#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_gpt_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : GPT HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 10.04.25
 ******************************************************************************************/

#ifndef SRC_HER2_GPT_HER2_GPT_DRV_H_
#define SRC_HER2_GPT_HER2_GPT_DRV_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(GPT_DRV)
#include "conf_gpt_drv.h"
#include "driver_api.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
    TMR_UNIT_PERIOD_RAW_COUNTS,  ///< Period in clock counts
    TMR_UNIT_PERIOD_NSEC,        ///< Period in nanoseconds
    TMR_UNIT_PERIOD_USEC,        ///< Period in microseconds
    TMR_UNIT_PERIOD_MSEC,        ///< Period in milliseconds
    TMR_UNIT_PERIOD_SEC,         ///< Period in seconds
    TMR_UNIT_FREQUENCY_HZ,       ///< Frequency in Hz
    TMR_UNIT_FREQUENCY_KHZ       ///< Frequency in kHz
} app_tmr_unit_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t gpt_init(void);
void GPT_DATA_RECEIVE(GptData *p_in_gpt_data);

#if(GPT_CH0)
void gpt_drv_configure0(void (*p_gpt0_event)(void));
app_err_t gpt_open_ch0(void);
app_err_t gpt_close_ch0(void);
app_err_t gpt_start_ch0(void);
app_err_t gpt_stop_ch0(void);
app_err_t gpt_setduty_ch0(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch0(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH1)
void gpt_drv_configure1(void (*p_gpt1_event)(void));
app_err_t gpt_open_ch1(void);
app_err_t gpt_close_ch1(void);
app_err_t gpt_start_ch1(void);
app_err_t gpt_stop_ch1(void);
app_err_t gpt_setduty_ch1(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch1(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH2)
void gpt_drv_configure2(void (*p_gpt2_event)(void));
app_err_t gpt_open_ch2(void);
app_err_t gpt_close_ch2(void);
app_err_t gpt_start_ch2(void);
app_err_t gpt_stop_ch2(void);
app_err_t gpt_setduty_ch2(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch2(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH3)
void gpt_drv_configure3(void (*p_gpt3_event)(void));
app_err_t gpt_open_ch3(void);
app_err_t gpt_close_ch3(void);
app_err_t gpt_start_ch3(void);
app_err_t gpt_stop_ch3(void);
app_err_t gpt_setduty_ch3(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch3(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH4)
void gpt_drv_configure4(void (*p_gpt4_event)(void));
app_err_t gpt_open_ch4(void);
app_err_t gpt_close_ch4(void);
app_err_t gpt_start_ch4(void);
app_err_t gpt_stop_ch4(void);
app_err_t gpt_setduty_ch4(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch4(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH5)
void gpt_drv_configure5(void (*p_gpt5_event)(void));
app_err_t gpt_open_ch5(void);
app_err_t gpt_close_ch5(void);
app_err_t gpt_start_ch5(void);
app_err_t gpt_stop_ch5(void);
app_err_t gpt_setduty_ch5(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch5(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH6)
void gpt_drv_configure6(void (*p_gpt6_event)(void));
app_err_t gpt_open_ch6(void);
app_err_t gpt_close_ch6(void);
app_err_t gpt_start_ch6(void);
app_err_t gpt_stop_ch6(void);
app_err_t gpt_setduty_ch6(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch6(uint16_t period, app_tmr_unit_t period_unit);
#endif

#if(GPT_CH7)
void gpt_drv_configure7(void (*p_gpt7_event)(void));
app_err_t gpt_open_ch7(void);
app_err_t gpt_close_ch7(void);
app_err_t gpt_start_ch7(void);
app_err_t gpt_stop_ch7(void);
app_err_t gpt_setduty_ch7(uint8_t pin, uint8_t duty);
app_err_t gpt_setperiod_ch7(uint16_t period, app_tmr_unit_t period_unit);
#endif

#endif
#endif
