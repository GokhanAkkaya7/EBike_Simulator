/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_threadx_utils.c
* Author       : Emre Erisgen
* Description  : Util functions related with ThreadX.
* Project      : VESTEL Common modules project
* Creation Date: 01.04.2021
******************************************************************************************/

/*------------------------------------ Includes ------------------------------------------*/

#include "jsn_threadx_utils.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: activate_oneshot_timer
 * Description  : Activates the given one shot application timer. First deactivate it, change its
 *                period and activate it.
 * Log_Code     : -
 * Arguments    : TX_TIMER *timer
 *                ULONG timeout
 * Return Value : bool
 ******************************************************************************************/
bool activate_oneshot_timer(TX_TIMER* p_timer, ULONG timeout)
{
    UINT status_t = 0u;
    status_t = tx_timer_deactivate(p_timer);
    if (TX_SUCCESS != status_t)
        return false;

    status_t = tx_timer_change(p_timer, timeout, 0);
    if (TX_SUCCESS != status_t)
        return false;

    status_t = tx_timer_activate(p_timer);
    if (TX_SUCCESS != status_t)
        return false;

    return true;
}

/***********************deactivate_oneshot_timer********************************************
 * Function Name: activate_oneshot_timer
 * Description  : Deactivates the given one shot application timer.
 * Log_Code     : -
 * Arguments    : TX_TIMER *timer
 * Return Value : bool
 ******************************************************************************************/
bool deactivate_oneshot_timer(TX_TIMER* p_timer)
{
    UINT status_t = 0u;
    status_t = tx_timer_deactivate(p_timer);
    if (TX_SUCCESS != status_t)
        return false;

    return true;
}

/******************************************************************************************
 * Function Name: change_periodic_timer
 * Description  : Changes period of the given periodic application timer. First deactivate it, change its
 *                period and activate it.
 * Log_Code     : -
 * Arguments    : TX_TIMER *timer
 *                ULONG timeout
 * Return Value : bool
 ******************************************************************************************/
bool change_periodic_timer(TX_TIMER* p_timer, ULONG timeout)
{
    UINT status_t = 0u;
    status_t = tx_timer_deactivate(p_timer);
    if (TX_SUCCESS != status_t)
        return false;

    status_t = tx_timer_change(p_timer, timeout, timeout);
    if (TX_SUCCESS != status_t)
        return false;

    status_t = tx_timer_activate(p_timer);
    if (TX_SUCCESS != status_t)
        return false;

    return true;
}

