#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_mx25_spi_fw.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : SPI (MX25 external module) HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 14.03.25
 ******************************************************************************************/

#ifndef SRC_HER2_SPI_MX25_SPI_HER2_MX25_SPI_FW_H_
#define SRC_HER2_SPI_MX25_SPI_HER2_MX25_SPI_FW_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(MX25_SPI_DRV)
#include "conf_mx25_spi.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*------------------------------------ Variables -----------------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t mx25_configure(void);
app_err_t mx25_open(void);
app_err_t mx25_close(void);
app_err_t mx25_write(uint32_t address, uint8_t* p_buffer, uint32_t length);
app_err_t mx25_read(uint32_t address, uint8_t* p_buffer, uint32_t length);
app_err_t mx25_sector_erase(uint32_t address);
app_err_t mx25_space_erase(uint32_t startAddress, uint32_t stopAddress);

#endif
#endif
