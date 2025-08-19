
/*------------------------------------ Includes ------------------------------------------*/

#include "her2_can_drv.h"

#if(CANBUS_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define CAN_WRITE_FAIL_RESET_LIMIT          100

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX can_lock;
static void (*p_can_rx_event)(app_can_frame_t*);
static TX_EVENT_FLAGS_GROUP* p_canbus_eflags = NULL;
static Message can_write_message;

/*---------------------------------- Global Variables ------------------------------------*/

extern TX_QUEUE g_outgoing_message_queue;

/*------------------------------- Private Function Prototypes ----------------------------*/

static void can_drv_reset(void);

#if CAN_RESET_TIMEOUT_CONTROL // TODO : What is this!
static void can_reopen_timer_callback(ULONG param);
#endif

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: can_drv_reset
 *
 * Description  : Reset CAN driver.
 *
 * Arguments    : void
 *
 * Return Value : None
 *******************************************************************************************/
static void can_drv_reset(void)
{
	// TODO GA: If smt be needed.
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: can_drv_configure
 *
 * Description  : Configure CAN Bus Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_canbus_event)(app_can_frame_type_t*)
 *                TX_EVENT_FLAGS_GROUP *p_canx_eflags
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t can_drv_configure(void (*p_canbus_rx_event)(app_can_frame_t*), TX_EVENT_FLAGS_GROUP* p_canx_eflags)
{
	p_can_rx_event = p_canbus_rx_event;
	p_canbus_eflags = p_canx_eflags;

	UINT tx_err = tx_mutex_create(&can_lock, (CHAR*)"CAN Bus Write Lock", TX_INHERIT);

	return APP_SUCCESS;
}

/******************************************************************************************
 * Function Name: can_drv_init_clock
 *
 * Description  : Initialize CAN clock.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *******************************************************************************************/
app_err_t can_drv_init_clock(void)
{
	// TODO GA:
	return APP_SUCCESS;
}

#if (CAN_RESET_TIMEOUT_CONTROL) // TODO : What is this!
/******************************************************************************************
 * Function Name: can_reopen_timer_callback
 *
 * Description  : Callback for opening can driver after reconnect timeout.
 *
 * Arguments    : ULONG param
 *
 * Return Value : None
 ******************************************************************************************/
static void can_reopen_timer_callback(ULONG param)
{
	SSP_PARAMETER_NOT_USED(param);
	can_drv_open();
}
#endif

/******************************************************************************************
 * Function Name: can_drv_open
 *
 * Description  : Opens CAN module.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t can_drv_open(void)
{
	UINT tx_res = tx_mutex_get(&can_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&can_lock);

	return result;
}

/******************************************************************************************
 * Function Name: can_drv_check_operation_state
 *
 * Description  : Checks CAN module operation state.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *                APP_CAN_MODE_NOT_NORMAL       can driver is not at normal mode
 *******************************************************************************************/
app_err_t can_drv_check_operation_state(void)
{
	UINT tx_res = tx_mutex_get(&can_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&can_lock);

	return result;
}

/******************************************************************************************
 * Function Name: can_drv_set_operation_state
 *
 * Description  : Sets CAN operation state.
 *
 * Arguments    : app_can_mode_t mode
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t can_drv_set_operation_state(app_can_mode_t mode)
{
	UINT tx_res = tx_mutex_get(&can_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	// TODO GA:

	tx_res = tx_mutex_put(&can_lock);

	return result;
}

/******************************************************************************************
 * Function Name: can_drv_close
 *
 * Description  : Closes the CAN module.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t can_drv_close(void)
{
	UINT tx_res = tx_mutex_get(&can_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&can_lock);

	return result;
}

/******************************************************************************************
 * Function Name: can_drv_write_frame
 *
 * Description  : Write data to CAN given mailbox.This call will return after all bytes are
 *                written or if a timeout occurs while waiting for access to the driver.
 *
 * Arguments    : app_can_frame_t *p_frame
 *                uint8_t mailbox
 *                uint8_t timeout
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_ASSERTION             p_src is null or bytes or timeout are equal to 0
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_CAN_WRITE_TIMEOUT     packet can't be send within the given time period
 *                APP_ERR_SSP_ERR               unexpected error on FSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t can_drv_write_frame(app_can_frame_t* p_frame, uint8_t mailbox, uint8_t timeout)
{
	UINT tx_res = tx_mutex_get(&can_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	can_write_message.driver = DRIVER_CAN;
	can_write_message.data.can.id = p_frame->id;
	can_write_message.data.can.dlc = p_frame->data_length_code;

	for (uint8_t can_indx = 0; can_indx < p_frame->data_length_code; can_indx++)
		can_write_message.data.can.can_buffer[can_indx] = p_frame->data[can_indx];

	Message* p_msg = &can_write_message;
	UINT status = tx_queue_send(&g_outgoing_message_queue, &p_msg, TX_NO_WAIT);

	tx_res = tx_mutex_put(&can_lock);
	return result;
}

/******************************************************************************************
 * Function Name: CANBUS_DRV_CALLBACK
 *
 * Description  : Callback function of CAN Driver.
 *
 * Arguments    : can_callback_args_t *p_args
 *
 * Return Value : None
 ******************************************************************************************/
void CANBUS_DRV_CALLBACK(Message* p_cb_data)
{
	app_can_frame_t rx_frame;

	rx_frame.id = p_cb_data->data.can.id;
	rx_frame.data_length_code = p_cb_data->data.can.dlc;

	for (uint8_t can_index = 0; can_index < 8; can_index++)
		rx_frame.data[can_index] = (uint8_t)p_cb_data->data.can.can_buffer[can_index];

	p_can_rx_event(&rx_frame);
}
#endif
