#pragma once
/******************************************************************************************
 * File Name    : conf_bq32000.h
 * Version      : v1.0
 * Device(s)    : S128
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Configuration file for TI BQ32000 module for the user application.
 * Project      : VESTEL Renesas Synergy Battery Project for Shimano Devices
 * Creation Date: 31.03.2021
 ******************************************************************************************/

#ifndef CONF_CONF_BQ32000_H_
#define CONF_CONF_BQ32000_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if (BQ32000)
#include "IIC/her_iic_fw.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define iic_bq3200_configure        iic1_fw_configure
#define iic_bq3200_init             iic1_fw_init
#define iic_bq3200_write            iic1_fw_write
#define iic_bq3200_read             iic1_fw_read

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
#endif
