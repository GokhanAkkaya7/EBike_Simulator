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

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_event_pubsub.h"

#if(EVENT_PUBSUB)
#include "jsn_log.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX event_pubsub_lock;
static const event_record_t* event_record_table;
static uint8_t* event_sub_table;
static uint8_t event_table_size;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static int8_t find_event_pos(uint8_t* p_event, TX_EVENT_FLAGS_GROUP* p_flag, uint32_t* p_mask);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: find_event_pos
 * Description  : Finds the position of the given event in given event record table.
 *
 * Arguments    : uint8_t *p_event,
 *                TX_EVENT_FLAGS_GROUP *p_flag,
 *                uint32_t *p_mask
 *                event_record_t p_event_table,
 *                uint8_t *p_event_table_size
 *
 * Return Value : int8_t : -1 if no record found,
 *                          0...n record index
 * ****************************************************************************************/
static int8_t find_event_pos(uint8_t* p_event, TX_EVENT_FLAGS_GROUP* p_flag, uint32_t* p_mask)
{
    int8_t res = -1;

    for (uint8_t indx = 0; indx < event_table_size && -1 == res; indx++)
        if (*p_event == event_record_table[indx].event && p_flag == event_record_table[indx].p_event_flag
            && *p_mask == event_record_table[indx].mask)
            res = (int8_t)indx;

    return res;
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: init_event_pubsub
 * Description  : Initialize event publish/subscribe module.
 * Arguments    : const uint8_t p_event_sub_table[],
 *                const event_record_t p_event_record_table[]
 * Return Value : bool
 * ****************************************************************************************/
bool init_event_pubsub(uint8_t* p_event_sub_table[], const event_record_t* p_event_record_table[], uint8_t event_size)
{
    UINT err_event_pubsub_lock;
    err_event_pubsub_lock = tx_mutex_create(&event_pubsub_lock, (CHAR*)"EventPubSub Lock", TX_INHERIT);
    if (TX_SUCCESS != err_event_pubsub_lock)
    {
        log_sw((uint8_t*)"init_event_pubsub: Event PubSub cant create lock", (uint16_t)err_event_pubsub_lock, error);
        return false;
    }

    event_sub_table = p_event_sub_table;
    event_record_table = p_event_record_table;
    event_table_size = event_size;

    return true;
}

/******************************************************************************************
 * Function Name: sub_event
 * Description  : Subscribe the event flag to the event with given mask.
 *
 * Arguments    : uint8_t event,
 *                TX_EVENT_FLAGS_GROUP *p_event_flag,
 *                uint32_t mask
 *
 * Return Value : bool : false if table is full
 * ****************************************************************************************/
bool sub_event(uint8_t event, TX_EVENT_FLAGS_GROUP* p_event_flag, uint32_t mask)
{
    tx_mutex_get(&event_pubsub_lock, TX_WAIT_FOREVER);

    // Find the position of event record.
    int8_t rindx = find_event_pos(&event, p_event_flag, &mask);
    bool b_res = true;

    if (-1 != rindx)  // insert the new record.
        event_sub_table[rindx] = 1;
    else
        b_res = false;

    tx_mutex_put(&event_pubsub_lock);

    return b_res;
}

/******************************************************************************************
 * Function Name: unsub_event
 * Description  : Un-subscribe the event flag to the event with given mask.
 *
 * Arguments    : uint8_t event,
 *                TX_EVENT_FLAGS_GROUP *p_event_flag,
 *                uint32_t mask
 *
 * Return Value : bool : false if there is no record
 * ****************************************************************************************/
bool unsub_event(uint8_t event, TX_EVENT_FLAGS_GROUP* p_event_flag, uint32_t mask)
{
    tx_mutex_get(&event_pubsub_lock, TX_WAIT_FOREVER);

    int8_t rindx = find_event_pos(&event, p_event_flag, &mask);
    bool b_res = true;

    if (-1 != rindx)
        event_sub_table[rindx] = 0;
    else
        b_res = false;

    tx_mutex_put(&event_pubsub_lock);

    return b_res;
}

/******************************************************************************************
 * Function Name: unsub_all_event
 * Description  : Un-subscribe the event flag from all events.
 *
 * Arguments    : TX_EVENT_FLAGS_GROUP *p_event_flag,
 *
 * Return Value : None
 * ****************************************************************************************/
void unsub_all_event(TX_EVENT_FLAGS_GROUP* p_event_flag)
{
    tx_mutex_get(&event_pubsub_lock, TX_WAIT_FOREVER);

    for (uint8_t indx = 0; indx < event_table_size; indx++)
        if (p_event_flag == event_record_table[indx].p_event_flag)
            event_sub_table[indx] = 0;

    tx_mutex_put(&event_pubsub_lock);
}

/***********************************************************************************************************************
 * Function Name: publish_event
 * Description  : Publishes the given event to its subscribers if any.
 *
 * Arguments    : uint8_t *event
 *                event_record_t p_event_table,
 *                uint8_t event_table_size
 *
 * Return Value : None
 * ***********************************************************************************************************************/
void publish_event(uint8_t event)
{
    tx_mutex_get(&event_pubsub_lock, TX_WAIT_FOREVER);

    for (uint8_t indx = 0; indx < event_table_size; indx++)
        if (event == event_record_table[indx].event && 1 == event_sub_table[indx])
            tx_event_flags_set(event_record_table[indx].p_event_flag, event_record_table[indx].mask, TX_OR);

    tx_mutex_put(&event_pubsub_lock);
}

#endif
