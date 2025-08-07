/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_io_drv.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : IO Driver module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 09.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_io_drv.h"

#if(IO_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define PIN_COUNT                           192

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX io_lock;
static app_io_level_t g_sim_pin_states[PIN_COUNT];
static bool g_is_initialized = false;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: io_init
 *
 * Description  : Initialize IO driver module, create mutex for locking operations.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *******************************************************************************************/
app_err_t io_init(void)
{
	if (g_is_initialized)
		return APP_SUCCESS;


	// Initialize all virtual pins to a known state (LOW).
	memset(g_sim_pin_states, APP_IO_LEVEL_LOW, sizeof(g_sim_pin_states));

	// Create a mutex for thread-safe access to the shared g_sim_pin_states array.
	UINT status = tx_mutex_create(&io_lock, (CHAR*)"IO Lock", TX_INHERIT);
	if (TX_SUCCESS != status)
		return APP_ERR_THREADX;


	g_is_initialized = true;
	return APP_SUCCESS;
}

/******************************************************************************************
 * Function Name: set_pin_level
 *
 * Description  : Sets the level of given IO pin according to the given parameter.
 *
 * Arguments    : app_io_port_pin_t pin
 *                app_io_level_t level
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t set_pin_level(app_io_port_pin_t pin, app_io_level_t level)
{
	// Lock the mutex to ensure exclusive access to the pin states array.
	UINT tx_res = tx_mutex_get(&io_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_initialized)
		result = APP_ERR_DRV_CONFIGURE;
	else
	{
		// Instead of a hardware call, we just update our memory array.
		g_sim_pin_states[pin] = level;

		result = APP_SUCCESS;
	}

	// Release the mutex.
	tx_res = tx_mutex_put(&io_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: get_pin_level
 *
 * Description  : Read the level of given IO pin.
 *
 * Arguments    : app_io_port_pin_t pin
 *                app_io_level_t *p_level
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_ASSERTION             Given parameter is null.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t get_pin_level(app_io_port_pin_t pin, app_io_level_t* p_level)
{
	// Lock the mutex to ensure exclusive access to the pin states array.
	UINT tx_res = tx_mutex_get(&io_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_initialized)
		result = APP_ERR_DRV_CONFIGURE;
	else if (NULL == p_level)
	{
		result = APP_ERR_ASSERTION;
	}
	else
	{
		// Read the state from our memory array.
		*p_level = g_sim_pin_states[pin];

		result = APP_SUCCESS;
	}

	// Release the mutex.
	tx_res = tx_mutex_put(&io_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: set_pin_ioout
 *
 * Description  : Sets the given pin to IO output pin.
 *
 * Arguments    : ioport_port_pin_t pin
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t set_pin_ioout(app_io_port_pin_t pin, app_io_level_t level)
{
	UINT tx_res = tx_mutex_get(&io_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	// There is no need to adjusting it like the way in hardware.
	app_err_t result =  set_pin_level(pin, level);

	// Release the mutex.
	tx_res = tx_mutex_put(&io_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: set_pin_perpsci1
 *
 * Description  : Sets the given pin to peripheral SCI1.
 *
 * Arguments    : app_io_port_pin_t pin,uint8_t channel_number
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_ASSERTION             Given parameter is null.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t set_pin_perpsci1(app_io_port_pin_t pin, uint8_t channel_number)
{
	UINT tx_res = tx_mutex_get(&io_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;
	
	// This function is highly hardware-specific and has no direct equivalent in a simple IO simulation.
	app_err_t result = APP_SUCCESS;

	tx_res = tx_mutex_put(&io_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: IO_DATAHANDLER
 *
 * Description  : Handle the incoming pin level request.
 *
 * Arguments    : IoData* p_incoming_data
 *
 * Return Value : None
 ******************************************************************************************/
void IO_DATAHANDLER(IoData* p_incoming_data)
{
	set_pin_level(p_incoming_data->pin, p_incoming_data->state);
}

#endif
