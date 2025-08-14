/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_monitor_log.c
* Author       : Emre Erisgen
* Description  : Common logger monitor module.
* Project      : VESTEL Common modules project
* Creation Date: 18.01.2023
******************************************************************************************/

/*------------------------------------ Includes ------------------------------------------*/

#include "jsn_monitor_log.h"

#if(COMMON_LOGGER && LOGGER_MONITOR)
#include "jsn_utils.h"
#include "jsn_log.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ---------------------------------*/

#define TIME_LOG_BUFF_SIZE                      32

/*------------------------------------ Static Variables ----------------------------------*/

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static void monitor_log_event(uint32_t data);

/*----------------------------------- Private Functions ----------------------------------*/

static void monitor_log_event(uint32_t data)
{
    SSP_PARAMETER_NOT_USED(data);
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: monitor_log_init
 * Description  : It sets monitor UART elements and register terminal send function to log module.
 * Arguments    : None
 * Return Value : None
 *******************************************************************************************/
bool monitor_log_init(void)
{
    if (!monitor_log_configure(&monitor_log_event, NULL))
        return false;

    if (!monitor_log_open())
        return false;

    return true;
}

/******************************************************************************************
 * Function Name: monitor_log_send
 * Description  : Sends logs to uart monitor.
 * Arguments    : uint8_t *p_log_str
 * Return Value : None
 *******************************************************************************************/
void monitor_log_send(uint8_t* p_log_str)
{
#if(LOG_TIME)
    uint8_t timeLogBuff[TIME_LOG_BUFF_SIZE] = { 0 };
    snprintf((char*)timeLogBuff, TIME_LOG_BUFF_SIZE, "%d/%d/%d - %d:%d:%d = ",
        p_log_str[5], p_log_str[4], p_log_str[3], p_log_str[6], p_log_str[7], p_log_str[8]);

    size_t timeLogSize = jsn_strlen(timeLogBuff);

    terminal_log_write(timeLogBuff, (uint32_t)timeLogSize, MONITOR_SEND_TIMEOUT);
#endif

    // Second element(p_log_str[1]) is log_size.
    monitor_log_write(&p_log_str[LOG_IND_SIZE], (uint32_t)(p_log_str[LOG_IND_SIZE_LOC] - LOG_IND_SIZE),
        MONITOR_SEND_TIMEOUT);
}

#endif
