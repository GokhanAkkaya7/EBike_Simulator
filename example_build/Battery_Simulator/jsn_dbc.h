#pragma once
/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her_dbc.h
 * Author       : Emre Erisgen
 * Description  : Debouncer module.
 * Project      : VESTEL Common modules project
 * Creation Date: 25.03.2022
 ******************************************************************************************/

#ifndef DEBOUNCER_JSN_DBC_H_
#define DEBOUNCER_JSN_DBC_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(DBC_APP)
#include "conf_dbc.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef struct
{
    uint8_t curr_btn_level;
    uint8_t desired_btn_level;
    uint8_t btn_pin;
    bool b_dbc_active;
    uint32_t cnt_pin;
    TX_TIMER* dbc_timer;
    ULONG dbc_interval;
    void (*dbc_irq_event)(void);
} dbc_irq_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

#if(DBC_CH0)
bool dbc0_configure(dbc_irq_t* p_dbc_irq);
void dbc0_flag_set(void);
#endif

#if(DBC_CH1)
bool dbc1_configure(dbc_irq_t* dbc_pin_param);
void dbc1_flag_set(void);
#endif

#if(DBC_CH2)
bool dbc2_configure(dbc_irq_t* dbc_pin_param);
void dbc2_flag_set(void);
#endif

#if(DBC_CH3)
bool dbc3_configure(dbc_irq_t* dbc_pin_param);
void dbc3_flag_set(void);
#endif

#if(DBC_CH4)
bool dbc4_configure(dbc_irq_t* dbc_pin_param);
void dbc4_flag_set(void);
#endif

#if(DBC_CH5)
bool dbc5_configure(dbc_irq_t* dbc_pin_param);
void dbc5_flag_set(void);
#endif

#if(DBC_CH6)
bool dbc6_configure(dbc_irq_t* dbc_pin_param);
void dbc6_flag_set(void);
#endif

#if(DBC_CH7)
bool dbc7_configure(dbc_irq_t* dbc_pin_param);
void dbc7_flag_set(void);
#endif

#endif

#endif
