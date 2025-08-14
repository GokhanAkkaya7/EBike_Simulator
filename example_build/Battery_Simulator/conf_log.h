#pragma once
/******************************************************************************************
 * File Name    : conf_log.h
 * Version      : v1.0
 * Device(s)    : S128
 * Tool-Chain   : GCC ARM Embedded 7.2.1.20170904
 * Description  : Configuration file for Common Logger for the user application.
 * Project      : VESTEL Common modules project
 * Creation Date: 09.08.2021
 ******************************************************************************************/

#ifndef CONF_CONF_LOG_H_
#define CONF_CONF_LOG_H_

 /*------------------------------------ Includes ------------------------------------------*/

 /*-------------------------------- Public  Macro Defines ---------------------------------*/

 // Warning: array size must not exceed this number in all logging operations!!!
#define LOG_QUEUE_SIZE                  64

#define LOGGER_PROGRAM_LOG              1
#define LOGGER_FRAME_LOG_1              VESTEL_LOG
#define LOGGER_FRAME_LOG_2              0
#define LOGGER_NUMBER                   (1 + VESTEL_LOG) // This number must be updated when a log field is closed!!!

#if DEBUG_COMMON_LOG
#define LIVE_DEBUB_LOG                  1                // Option for printing logs to given interface instead of flash
#define PURE_LOG                        1                // Option for printing logs without log type and code
#else
#define LIVE_DEBUB_LOG                  0                // Option for printing logs to given interface instead of flash
#define PURE_LOG                        0                // Option for printing logs without log type and code
#endif

//******************** program log configuration area ********************//
#if(LOGGER_PROGRAM_LOG)
#if(LIVE_DEBUB_LOG)
#define LOGGER_PLOG_FLASH               0
#define LOGGER_PLOG_MONITOR             1
#else
#define LOGGER_PLOG_FLASH               1
#define LOGGER_PLOG_MONITOR             0
#endif
#define LOGGER_PLOG_TIME                0
#define LOGGER_PLOG_INDEX               0
#if(LOGGER_PLOG_FLASH)
#define LOGGER_PLOG_START_OFFSET        0x080000
#define LOGGER_PLOG_SIZE                0xFFFF      //64kb
#define LOGGER_PLOG_OVERWRITE           1
#endif
#endif
//**************************************************************************//

//******************** vestel log configuration area ********************//
#if(LOGGER_FRAME_LOG_1)
#define LOGGER_FRAME1_FLASH             1
#define LOGGER_FRAME1_MONITOR           0
#define LOGGER_FRAME1_TIME              0
#define LOGGER_FRAME1_INDEX             1
#define LOGGER_FRAME1_FRAME_SIZE        61
#if(LOGGER_FRAME1_FLASH)
#define LOGGER_FRAME1_START_OFFSET      0x090000
#define LOGGER_FRAME1_SIZE              0xFFFF      //64kb
#define LOGGER_FRAME1_OVERWRITE         0
#endif


#define VESTEL_LOG_INDEX                LOGGER_FRAME1_INDEX
#define VESTEL_LOG_SIZE                 LOGGER_FRAME1_FRAME_SIZE
#define VESTEL_LOG_PERIOD               1  // second
#define VESTEL_LOG_AUTOSTART            0
#endif
//**************************************************************************//

#if(LOGGER_FRAME_LOG_2)
#define LOGGER_FRAME2_FLASH             1
#define LOGGER_FRAME2_MONITOR           0
#define LOGGER_FRAME2_TIME              0
#define LOGGER_FRAME2_INDEX             2
#define LOGGER_FRAME2_FRAME_SIZE        0
#if(LOGGER_FRAME2_FLASH)
#define LOGGER_FRAME2_START_OFFSET      0x0C0000
#define LOGGER_FRAME2_SIZE              0xFFFF      //64kb
#define LOGGER_FRAME2_OVERWRITE         0
#endif
#endif

#if defined(DIOMEDES)
#define LOGGER_MONITOR                  0    // TODO: Diomedes : Enable monitor log for diomedes when terminal is ready
#else
#define LOGGER_MONITOR                  0    //TODO GA: Open it later. // This define must be "1" when any log use monitor!!!
#endif

#if(LOGGER_MONITOR)

#if CAN_LOG_STREAM
// TODO: BK: Assign terminal in conf project settings instead of compiler switch
#if defined(BAFANG_CAN) || defined (BAFANG_CAN_6_7)
#include "TerminalCan/BafangCan/ykz_term_can_bafang.h"
#endif

#if defined(ANANDA_CAN)
#include "TerminalCan/AnandaCan/ykz_term_can_ananda.h"
#endif

#if defined(MIVICE_CAN) || defined(MIVICE_CAN_A68)
#include "TerminalCan/MiviceCan/ykz_term_can_mivice.h"
#endif

#define monitor_log_configure           can_log_init
#define monitor_log_open                enable_can_log_stream
#define monitor_log_close               disable_can_log_stream
#define monitor_log_write               send_can_log

#if DUAL_BATTERY_MODE && defined(IF01)
#define monitor_log_configure           sci9_drv_configure
#define monitor_log_open                sci9_drv_open
#define monitor_log_close               sci9_drv_close
#define monitor_log_write               sci9_drv_write
#elif defined(IF01) || (defined(IF04) && (!CAN_APP))
#define monitor_log_configure           sci2_drv_configure
#define monitor_log_open                sci2_drv_open
#define monitor_log_close               sci2_drv_close
#define monitor_log_write               sci2_drv_write
#endif
#endif
#endif

#if(LOG_CAN_INTERFACE)
#include "TerminalCan/CanLogInterface/can_log_interface.h"
#define interface_write                 can_log_write
#else
#define interface_write                 NULL
#endif

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif
