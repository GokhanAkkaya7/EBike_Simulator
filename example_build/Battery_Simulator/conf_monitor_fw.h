#pragma once
/******************************************************************************************
 * File Name    : conf_monitor_fw.h
 * Version      : v1.0
 * Device(s)    : S128
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Configuration file for Monitor FW for the user application.
 * Project      : VESTEL Renesas Synergy Battery Project for Shimano Devices
 * Creation Date: 15 Nis 2021
 ******************************************************************************************/

#ifndef CONF_CONF_MONITOR_FW_H_
#define CONF_CONF_MONITOR_FW_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define DEBUG_WDT_FEATURE					0      // it makes it possible for wdt to work in debug.

#define MONITOR_FW_NAME						thread_monitor_fw

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
