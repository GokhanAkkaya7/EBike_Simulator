#pragma once
/******************************************************************************************
 * File Name    : conf_aes.h
 * Version      : v1.0
 * Device(s)    : S128
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Configuration file for AES module for the user application.
 * Project      : VESTEL Renesas Synergy Battery Project for Shimano Devices
 * Creation Date: 31.03.2021
 ******************************************************************************************/

#ifndef CONF_CONF_AES_H_
#define CONF_CONF_AES_H_

 /*------------------------------------ Includes ------------------------------------------*/

 /*-------------------------------- Public  Macro Defines ---------------------------------*/

#define AES_KEY { 0x23, 0xE4, 0xE8, 0xFA, 0x03, 0x06, 0x86, 0xF2, 0xD8, 0x5D, 0xAB, 0xD6, 0xA7, 0x43, 0xD3, 0x95 } // Shimano key.

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
