/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : conf_gpt_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Configuration file of GPT Driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 10.04.25
 ******************************************************************************************/

#ifndef SRC_CONF_CONF_GPT_DRV_H_
#define SRC_CONF_CONF_GPT_DRV_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(GPT_DRV)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define GPT_CH0								1
#define GPT_CH1								1
#define GPT_CH2								1
#define GPT_CH3 							1
#define GPT_CH4								1
#define GPT_CH5								1
#define GPT_CH6								1
#define GPT_CH7								1
#define GPT_CH8								1
#define GPT_CH9								1

#define GPT0_CALLBACK						1
#define GPT1_CALLBACK						1
#define GPT2_CALLBACK						1
#define GPT3_CALLBACK						1
#define GPT4_CALLBACK						1
#define GPT5_CALLBACK						1
#define GPT6_CALLBACK						1
#define GPT7_CALLBACK						1

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
#endif
