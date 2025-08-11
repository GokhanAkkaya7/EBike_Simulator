#pragma once
/******************************************************************************************
 * © Copyright [2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : conf_ringbuffer.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas RA Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Conf file of ring buffer for cbs application.
 * Project      : CBS Application Project
 * Creation Date: 21.03.24
 ******************************************************************************************/

#ifndef SRC_CONF_CONF_RINGBUFFER_H_
#define SRC_CONF_CONF_RINGBUFFER_H_

 /*------------------------------------ Includes ------------------------------------------*/

 /*-------------------------------- Public  Macro Defines ---------------------------------*/

#define RING_BUFFER_MAX                     1
#define OPERATION_BUFFER_SIZE               1
#define OPERATIN_WORD_SIZE                  1

#define RING_BUFFER_INDEX_ZERO              0

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
	can_log_stream = 0,
} rb_idx_e;

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
