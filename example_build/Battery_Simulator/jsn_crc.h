#pragma once
/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_crc.h
* Author       : Emre Erisgen
* Description  : CRC hash module.
* Project      : VESTEL Common modules project
* Creation Date: 31.03.2021
******************************************************************************************/

#ifndef GMD_CRYPTO_GMD_CRC_H_
#define GMD_CRYPTO_GMD_CRC_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"
#if(CRC)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

unsigned short crc_apigen(const unsigned char* const dat_ptr, const unsigned char dat_len);
unsigned char crc_apichk(const unsigned char* const dat_ptr, const unsigned char dat_len);
uint8_t calculate_crc8(uint8_t* input, size_t length);
uint32_t crc32_compute(uint8_t const* const dat_ptr, size_t const dat_len);
uint32_t crc32_mpeg2_compute(uint8_t const* const dat_ptr, size_t const dat_len);
uint16_t crc16(uint8_t* pBuf, uint8_t nLen);

#endif
#endif
