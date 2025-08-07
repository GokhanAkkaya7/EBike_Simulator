#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_flash_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Flash Driver HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 14.04.25
 ******************************************************************************************/

#ifndef SRC_HER2_FLASH_HER2_FLASH_DRV_H_
#define SRC_HER2_FLASH_HER2_FLASH_DRV_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(FLASH_DRV)
#include "conf_flash_drv.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define DATAFLASH_BLOCK_TOTAL_NUMBER		(64)				// There is 64 block in total (64 * 1kB = 64 kB).
#define DATAFLASH_BLOCK_SIZE                (0x400)             // Data Flash block Size: 1kB
#define FLASH_DF_BLOCK_START                (0x00000000)        // Data Flash blocks start address.
#define FLASH_DF_BLOCK_END                  (0x00010000)        // Data Flash blocks end address.

#define CODEFLASH_BLOCK_SIZE                (0x800)             // Code Flash block Size: 2kB
#define FLASH_CF_BLOCK_START                (0x00000000)        // Code Flash blocks start address.
#define FLASH_CF_BLOCK_END                  (0x00040000)        // Code Flash blocks end address.

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t flash_drv_configure(void);
app_err_t flash_drv_init(void);

/* Data Flash Methods */
app_err_t dflash_drv_block_write(uint8_t const* const p_write_buffer, uint32_t block_address, uint32_t num_bytes);
app_err_t dflash_drv_block_read(uint8_t* p_read_buffer, uint32_t block_address, uint32_t num_bytes);

/* Code Flash Methods */
app_err_t cflash_drv_block_erase(uint32_t block_address);
app_err_t cflash_drv_block_write(uint8_t const* const p_write_buffer, uint32_t block_address, uint32_t num_bytes);
app_err_t cflash_drv_block_read(uint8_t* p_read_buffer, uint32_t block_address, uint32_t num_bytes);
app_err_t cflash_drv_block_blank_check(uint32_t block_address, bool* p_is_blank);
void FLASH_DRV_CALLBACK(void);

#endif
#endif
