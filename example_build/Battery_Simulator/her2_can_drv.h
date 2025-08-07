#pragma once

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(CANBUS_DRV)
#include "conf_canbus_drv.h"
#include "driver_api.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define CAN_EVENT_ERR_WARNING_FLAG          0x01
#define CAN_EVENT_ERR_PASSIVE_FLAG          0x02
#define CAN_EVENT_ERR_BUS_OFF_FLAG          0x04
#define CAN_EVENT_BUS_RECOVERY_FLAG         0X08
#define CAN_EVENT_MAILBOX_MESSAGE_LOST_FLAG 0x10
#define CAN_EVENT_ERR_BUS_LOCK_FLAG         0x20
#define CAN_EVENT_ERR_CHANNEL_FLAG          0x40
#define CAN_EVENT_TX_ABORTED_FLAG           0X80
#define CAN_EVENT_ERR_GLOBAL_FLAG           0x100
#define CAN_EVENT_FIFO_MESSAGE_LOST_FLAG    0x200
#define CAN_EVENT_TX_FIFO_EMPTY_FLAG        0X400

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef uint32_t appcan_id_t;

typedef enum e_appcan_frame_type
{
	APP_CAN_FRAME_TYPE_DATA,            ///< Data frame type.
	APP_CAN_FRAME_TYPE_REMOTE,          ///< Remote frame type.
} app_can_frame_type_t;

typedef struct st_appcan_frame
{
	appcan_id_t id;                     ///< CAN id.
	uint8_t data_length_code;           ///< CAN Data Length code, number of bytes in the message.
	uint8_t data[8];                    ///< CAN data, up to 8 bytes.
	app_can_frame_type_t type;          ///< Frame type, data or remote frame.
} app_can_frame_t;

typedef enum e_appcan_mode
{
	APP_CAN_MODE_NORMAL,                ///< CAN Normal Mode.
	APP_CAN_MODE_HALT,                  ///< CAN Halt Mode.
	APP_CAN_MODE_SLEEP,                 ///< CAN SLEEP Mode.
	APP_CAN_MODE_EXIT_SLEEP,            ///< CAN Exit SLEEP Mode.
	APP_CAN_MODE_RESET,                 ///< CAN Reset Mode.
	APP_CAN_MODE_LISTEN,                ///< CAN Listen Mode.
	APP_CAN_MODE_LOOPBACK_INTERNAL,     ///< CAN Internal Loopback Mode.
	APP_CAN_MODE_LOOPBACK_EXTERNAL,     ///< CAN External Loopback Mode.
} app_can_mode_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t can_drv_configure(void (*p_canbus_rx_event)(app_can_frame_t*), TX_EVENT_FLAGS_GROUP* p_canx_eflags);
app_err_t can_drv_open(void);
app_err_t can_drv_close(void);
app_err_t can_drv_init_clock(void);
app_err_t can_drv_write_frame(app_can_frame_t* p_frame, uint8_t mailbox, uint8_t timeout);
app_err_t can_drv_check_operation_state(void);
app_err_t can_drv_set_operation_state(app_can_mode_t mode);
void CANBUS_DRV_CALLBACK(Message* p_cb_data);

#if (DEBUG_CANBUS)
void can_drv_clear_flags(void);
#endif

#endif