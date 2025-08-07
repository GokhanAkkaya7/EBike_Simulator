/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_adc_fw.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Environment  : Microsoft Visual Studio 2022, Version 17.5.3.
 * Description  : ADC Microsoft Layer.
 * Project      : VESTEL Renesas Microsoft Layer.
 * Creation Date: 29.05.25
 ******************************************************************************************/

#pragma once

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(ADC_FW)
#include "conf_adc_fw.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#if(ADC0_FW)
app_err_t adc0_fw_configure(void (*p_adc_event)(uint8_t, uint16_t*));
app_err_t adc0_fw_open(void);
app_err_t adc0_fw_start(void);
app_err_t adc0_fw_stop(void);
app_err_t adc0_fw_close(void);
void ADC0_FW_CALLBACK(uint8_t index, uint16_t* adc_buffer);
#endif

#endif