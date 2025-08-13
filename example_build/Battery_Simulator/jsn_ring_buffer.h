#pragma once
/******************************************************************************************
 * © Copyright [2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_ring_buffer.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas RA Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Ring Buffer Module.
 * Project      : VESTEL Common modules project.
 * Creation Date: 21.03.24
 ******************************************************************************************/

#ifndef SRC_JSN_RINGBUFFER_JSN_RING_BUFFER_H_
#define SRC_JSN_RINGBUFFER_JSN_RING_BUFFER_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(RINGBUFFER)
#include "conf_ringbuffer.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define LOG_DATA_BUF_SIZE           400

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef struct
{
    size_t s_elem;
    size_t n_elem;
    void* buffer;

} rb_attr_t;


/*----------------------------- Public Function Prototypes -------------------------------*/

/*************************** Ring Buffer Implementation Type 1 ****************************/
bool ring_buffer_configure(rb_attr_t* attr_str, uint8_t word_size, uint16_t buff_size, uint8_t* rb_buff);
bool ring_buffer_init(rb_idx_e rb_idx, rb_attr_t* attr);
bool ring_buffer_notify(rb_idx_e rb_idx, void (*rb_notify)(void));
void ring_buffer_get(rb_idx_e rb_idx, void* data);
void ring_buffer_put(rb_idx_e rb_idx, const void* data);
uint16_t get_rbsize_status(rb_idx_e rb_number);

/*************************** Ring Buffer Implementation Type 2 ****************************/
void rb_char_put(char* data);
void rb_one_char_put(char data);
uint8_t* rb_char_get(uint16_t* c_size);

#endif
#endif
