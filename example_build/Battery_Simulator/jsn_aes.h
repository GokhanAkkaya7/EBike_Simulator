#pragma once
/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_aes.h
 * Author       : Gokhan Akkaya
 * Description  : AES encryption module.
 * Project      : VESTEL Common modules project
 * Creation Date: 31.03.2021
 ******************************************************************************************/

#ifndef JSN_CRYPTO_JSN_AES_H_
#define JSN_CRYPTO_JSN_AES_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"
#if(AES)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

void init_aes(void);
void encrypt_aes_128(unsigned char* plain);
void decrypt_aes_128(unsigned char* cipher);

#endif
#endif
