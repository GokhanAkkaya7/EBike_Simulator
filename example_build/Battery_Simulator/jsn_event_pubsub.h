#pragma once
/******************************************************************************************
 * © Copyright [2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_event_pubsub.c
 * Author       : Gokhan Akkaya
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Event Publish/Subscribe module.
 * Project      : VESTEL Common modules project
 * Creation Date: 30.04.21
 ******************************************************************************************/

#ifndef SRC_JSN_EVENTPUBSUB_JSN_EVENT_PUBSUB_H_
#define SRC_JSN_EVENTPUBSUB_JSN_EVENT_PUBSUB_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(EVENT_PUBSUB)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef struct
{
    uint8_t event;
    TX_EVENT_FLAGS_GROUP* p_event_flag;
    uint32_t mask;
} event_record_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

bool init_event_pubsub(uint8_t p_event_sub_table[], const event_record_t p_event_record_table[], uint8_t event_size);
bool sub_event(uint8_t event, TX_EVENT_FLAGS_GROUP* p_event_flag, uint32_t mask);
bool unsub_event(uint8_t event, TX_EVENT_FLAGS_GROUP* p_event_flag, uint32_t mask);
void unsub_all_event(TX_EVENT_FLAGS_GROUP* p_event_flag);
void publish_event(uint8_t event);

#endif
#endif
