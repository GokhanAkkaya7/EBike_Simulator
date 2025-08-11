#pragma once
/******************************************************************************************
 * File Name    : conf_dbc.h
 * Version      : v1.0
 * Device(s)    : S128
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Configuration file for Debouncer app for the user application.
 * Project      : VESTEL Renesas Synergy Battery Project for Shimano Devices
 * Creation Date: 17.03.2021
 ******************************************************************************************/

#ifndef CONF_CONF_DBC_H_
#define CONF_CONF_DBC_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_irq_drv.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define DBC_CH0             1
#define DBC_CH1             0
#define DBC_CH2             0
#define DBC_CH3             0
#define DBC_CH4             0
#define DBC_CH5             0
#define DBC_CH6             0
#define DBC_CH7             0

#if(DBC_CH0)
#define DBC0_NAME			button_irq_driver
#define DBC0_CALLBACK		button_irq_callback
#endif

#if(DBC_CH1)
#define DBC1_NAME			lorem_ipsum
#define DBC1_CALLBACK		dolor_sit_amet
#endif

#if(DBC_CH2)
#define DBC2_NAME			lorem_ipsum
#define DBC2_CALLBACK		dolor_sit_amet
#endif

#if(DBC_CH3)
#define DBC3_NAME			lorem_ipsum
#define DBC3_CALLBACK		dolor_sit_amet
#endif

#if(DBC_CH4)
#define DBC4_NAME			lorem_ipsum
#define DBC4_CALLBACK		dolor_sit_amet
#endif

#if(DBC_CH5)
#define DBC5_NAME			lorem_ipsum
#define DBC5_CALLBACK		dolor_sit_amet
#endif

#if(DBC_CH6)
#define DBC6_NAME			bttx_echo_driver
#define DBC6_CALLBACK		bttx_irq_callback
#endif

#if(DBC_CH7)
#define DBC7_NAME			btrx_echo_driver
#define DBC7_CALLBACK		btrx_irq_callback
#endif

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/


#endif
