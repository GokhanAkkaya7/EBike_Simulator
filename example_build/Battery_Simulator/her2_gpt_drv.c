/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_gpt_drv.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : GPT HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 10.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_gpt_drv.h"

#if(GPT_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

typedef struct
{
	uint8_t channel_no;
	BOOL is_configured;
	BOOL is_open;
	TX_TIMER tx_timer;
	ULONG initial_period_ms;
	void (*app_callback)(void);
	CHAR* timer_name;
} simulated_gpt_instance_t;

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define MAX_GPT_CHANNELS					8 

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX gpt_lock;
static TX_MUTEX* p_gpt_lock = NULL;

static simulated_gpt_instance_t g_sim_gpts[MAX_GPT_CHANNELS];

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static ULONG _timeunit_to_ms(GptUnit_e in_unit, int time_value);
static ULONG _ms_to_ticks(ULONG ms);
static void generic_gpt_callback(ULONG channel);
static app_err_t gpt_open_ch(simulated_gpt_instance_t* p_inst);
static app_err_t gpt_close_ch(simulated_gpt_instance_t* p_inst);
static app_err_t gpt_start_ch(simulated_gpt_instance_t* p_inst);
static app_err_t gpt_stop_ch(simulated_gpt_instance_t* p_inst);
static app_err_t gpt_setduty_ch(simulated_gpt_instance_t* p_inst, uint8_t pin, uint8_t duty);
static app_err_t gpt_setperiod_ch(simulated_gpt_instance_t* p_inst, uint16_t period, app_tmr_unit_t period_unit);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: _timeunit_to_ms
 *
 * Description  : Convert the given ttimer unit value to miliseconds.
 *
 * Arguments    : GptUnit_e in_unit, int time_value
 *
 * Return Value : ULONG
 ******************************************************************************************/
static ULONG _timeunit_to_ms(GptUnit_e in_unit, int time_value)
{
	ULONG result = 0;

	switch (in_unit)
	{
	case Seconds:
		result = 1000 * time_value;
		break;
	case Milliseconds:
		result = time_value;
		break;
	default:
		break;
	}

	return result;
}

/******************************************************************************************
 * Function Name: _ms_to_ticks
 *
 * Description  : Convert the given ms value to timer_ticks.
 *
 * Arguments    : ULONG ms
 *
 * Return Value : ULONG
 ******************************************************************************************/
static ULONG _ms_to_ticks(ULONG ms)
{
	if (ms == 0)
		return 1;

	return (ms * TX_TIMER_TICKS_PER_SECOND) / 1000;
}

/******************************************************************************************
 * Function Name: gpt_open_ch
 *
 * Description  : Open given GPT Channel.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t gpt_open_ch(simulated_gpt_instance_t* p_inst)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_ERR_FSP_ERR;

	if (p_inst->is_open)
		result = APP_SUCCESS;
	else
	{
		ULONG ticks = _ms_to_ticks(p_inst->initial_period_ms);

		UINT status = tx_timer_create(&p_inst->tx_timer, p_inst->timer_name,
			generic_gpt_callback, p_inst->channel_no,
			ticks, ticks, TX_NO_ACTIVATE);

		if (status != TX_SUCCESS)
			result = APP_ERR_THREADX;
		else
		{
			p_inst->is_open = TRUE;
			result = APP_SUCCESS;
		}
	}

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: gpt_close_ch
 *
 * Description  : Close given GPT Channel.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t gpt_close_ch(simulated_gpt_instance_t* p_inst)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	if (!p_inst->is_open)
		result = APP_SUCCESS;

	tx_timer_delete(&p_inst->tx_timer);
	p_inst->is_open = FALSE;

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: gpt_start_ch
 *
 * Description  : Start GPT given Channel.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t gpt_start_ch(simulated_gpt_instance_t* p_inst)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	if (!p_inst->is_open)
		result = APP_FAIL;
	else
		if (TX_SUCCESS == tx_timer_activate(&p_inst->tx_timer))
			result = APP_SUCCESS;
		else
			result = APP_ERR_THREADX;

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: gpt_stop_ch
 *
 * Description  : Stop GPT given Channel.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t gpt_stop_ch(simulated_gpt_instance_t* p_inst)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	if (!p_inst->is_open)
		result = APP_FAIL;
	else
		if (TX_SUCCESS == tx_timer_deactivate(&p_inst->tx_timer))
			result = APP_SUCCESS;
		else
			result = APP_ERR_THREADX;

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch
 *
 * Description  : Sets the given PWM duty to the given pins.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *                uint8_t pin
 *                uint8_t duty
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *****************************************************************************************/
static app_err_t gpt_setduty_ch(simulated_gpt_instance_t* p_inst, uint8_t pin, uint8_t duty)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch
 *
 * Description  : Sets the period of the timer.
 *
 * Arguments    : simulated_gpt_instance_t* p_inst
 *                uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *****************************************************************************************/
static app_err_t gpt_setperiod_ch(simulated_gpt_instance_t* p_inst, uint16_t period, app_tmr_unit_t period_unit)
{
	UINT tx_res = tx_mutex_get(&gpt_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;
	// TODO GA: do it according to period_unit instead of directly converting it to ms.
	if (!p_inst->is_open)
		result = APP_FAIL;

	ULONG ticks = _ms_to_ticks(period);

	if (TX_SUCCESS == tx_timer_change(&p_inst->tx_timer, ticks, ticks))
		result = APP_SUCCESS;
	else
		result = APP_ERR_THREADX;

	tx_res = tx_mutex_put(&gpt_lock);

	return result;
}

/******************************************************************************************
 * Function Name: generic_gpt_callback
 *
 * Description  : General callback of the gpt.
 *
 * Arguments    : ULONG channel
 *
 * Return Value : None
 ****************************************************************************************/
static void generic_gpt_callback(ULONG channel)
{
	if (channel >= MAX_GPT_CHANNELS)
		return;

	simulated_gpt_instance_t* p_instance = &g_sim_gpts[channel];

	if (p_instance->is_configured && p_instance->app_callback)		// Check if there is r4ecorded callback.
		p_instance->app_callback();

}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: gpt_init
 *
 * Description  : Initialize GPT driver module, create mutex for locking operations.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ****************************************************************************************/
app_err_t gpt_init(void)
{
	if (NULL != p_gpt_lock)
		return APP_SUCCESS;

	UINT err_gpt_lock = tx_mutex_create(&gpt_lock, (CHAR*)"GPT Driver Lock", TX_INHERIT);
	if (TX_SUCCESS != err_gpt_lock)
	{
		return APP_ERR_THREADX;
	}
	else
	{
		p_gpt_lock = &gpt_lock;
		return APP_SUCCESS;
	}
}

#if(GPT_CH0)
/******************************************************************************************
 * Function Name: gpt_drv_configure0
 *
 * Description  : Configure GPT Channel 0 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt0_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure0(void (*p_gpt0_event)(void))
{
	g_sim_gpts[0].timer_name = "gpt0_timer";
	g_sim_gpts[0].channel_no = 0;
#ifdef GPT0_CALLBACK
	g_sim_gpts[0].app_callback = p_gpt0_event;
#endif

	g_sim_gpts[0].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch0
 *
 * Description  : Open GPT Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *****************************************************************************************/
app_err_t gpt_open_ch0(void)
{
	return gpt_open_ch(&g_sim_gpts[0]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch0
 *
 * Description  : Close GPT Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch0(void)
{
	return gpt_close_ch(&g_sim_gpts[0]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch0
 *
 * Description  : Start the GPT Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch0(void)
{
	return gpt_start_ch(&g_sim_gpts[0]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch0
 *
 * Description  : Start the GPT Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch0(void)
{
	return gpt_stop_ch(&g_sim_gpts[0]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch0
 *
 * Description  : Set duty of the GPT Channel 0.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setduty_ch0(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[0], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch0
 *
 * Description  : Set period of the GPT Channel 0.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setperiod_ch0(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[0], period, period_unit);
}
#endif

#if(GPT_CH1)
/******************************************************************************************
 * Function Name: gpt_drv_configure1
 *
 * Description  : Configure GPT Channel 1 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt1_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure1(void (*p_gpt1_event)(void))
{
	g_sim_gpts[1].timer_name = "gpt1_timer";
	g_sim_gpts[1].channel_no = 1;
#ifdef GPT1_CALLBACK
	g_sim_gpts[1].app_callback = p_gpt1_event;
#endif

	g_sim_gpts[1].is_configured = TRUE;

}

/******************************************************************************************
 * Function Name: gpt_open_ch1
 *
 * Description  : Open GPT Channel 1.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch1(void)
{
	return gpt_open_ch(&g_sim_gpts[1]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch1
 *
 * Description  : Close GPT Channel 1.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch1(void)
{
	return gpt_close_ch(&g_sim_gpts[1]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch1
 *
 * Description  : Start the GPT Channel 1.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch1(void)
{
	return gpt_start_ch(&g_sim_gpts[1]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch1
 *
 * Description  : Start the GPT Channel 1.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch1(void)
{
	return gpt_stop_ch(&g_sim_gpts[1]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch1
 *
 * Description  : Set duty of the GPT Channel 1.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setduty_ch1(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[1], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch1
 *
 * Description  : Set period of the GPT Channel 1.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setperiod_ch1(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[1], period, period_unit);
}
#endif

#if(GPT_CH2)
/******************************************************************************************
 * Function Name: gpt_drv_configure2
 *
 * Description  : Configure GPT Channel 2 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt2_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure2(void (*p_gpt2_event)(void))
{
	g_sim_gpts[2].timer_name = "gpt2_timer";
	g_sim_gpts[2].channel_no = 2;
#ifdef GPT2_CALLBACK
	g_sim_gpts[2].app_callback = p_gpt2_event;
#endif

	g_sim_gpts[2].is_configured = TRUE;

}

/******************************************************************************************
 * Function Name: gpt_open_ch2
 *
 * Description  : Open GPT Channel 2.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch2(void)
{
	return gpt_open_ch(&g_sim_gpts[2]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch2
 *
 * Description  : Close GPT Channel 2.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch2(void)
{
	return gpt_close_ch(&g_sim_gpts[2]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch2
 *
 * Description  : Start the GPT Channel 2.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch2(void)
{
	return gpt_start_ch(&g_sim_gpts[2]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch2
 *
 * Description  : Start the GPT Channel 2.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch2(void)
{
	return gpt_stop_ch(&g_sim_gpts[2]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch2
 *
 * Description  : Set duty of the GPT Channel 2.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setduty_ch2(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[2], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch2
 *
 * Description  : Set period of the GPT Channel 2.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setperiod_ch2(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[2], period, period_unit);
}
#endif

#if(GPT_CH3)
/******************************************************************************************
 * Function Name: gpt_drv_configure3
 *
 * Description  : Configure GPT Channel 3 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt3_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure3(void (*p_gpt3_event)(void))
{
	g_sim_gpts[3].timer_name = "gpt3_timer";
	g_sim_gpts[3].channel_no = 3;
#ifdef GPT3_CALLBACK
	g_sim_gpts[3].app_callback = p_gpt3_event;
#endif

	g_sim_gpts[3].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch3
 *
 * Description  : Open GPT Channel 3.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch3(void)
{
	return gpt_open_ch(&g_sim_gpts[3]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch3
 *
 * Description  : Close GPT Channel 3.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch3(void)
{
	return gpt_close_ch(&g_sim_gpts[3]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch3
 * Description  : Start the GPT Channel 3.
 * Arguments    : None
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch3(void)
{
	return gpt_start_ch(&g_sim_gpts[3]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch3
 *
 * Description  : Start the GPT Channel 3.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch3(void)
{
	return gpt_stop_ch(&g_sim_gpts[3]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch3
 *
 * Description  : Set duty of the GPT Channel 3.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setduty_ch3(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[3], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch3
 *
 * Description  : Set period of the GPT Channel 3.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setperiod_ch3(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[3], period, period_unit);
}
#endif

#if(GPT_CH4)
/******************************************************************************************
 * Function Name: gpt_drv_configure4
 *
 * Description  : Configure GPT Channel 4 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt4_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure4(void (*p_gpt4_event)(void))
{
	g_sim_gpts[4].timer_name = "gpt4_timer";
	g_sim_gpts[4].channel_no = 4;
#ifdef GPT4_CALLBACK
	g_sim_gpts[4].app_callback = p_gpt4_event;
#endif

	g_sim_gpts[4].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch4
 *
 * Description  : Open GPT Channel 4.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch4(void)
{
	return gpt_open_ch(&g_sim_gpts[4]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch4
 *
 * Description  : Close GPT Channel 4.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch4(void)
{
	return gpt_close_ch(&g_sim_gpts[4]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch4
 *
 * Description  : Start the GPT Channel 4.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch4(void)
{
	return gpt_start_ch(&g_sim_gpts[4]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch4
 *
 * Description  : Start the GPT Channel 4.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch4(void)
{
	return gpt_stop_ch(&g_sim_gpts[4]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch4
 *
 * Description  : Set duty of the GPT Channel 4.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setduty_ch4(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[4], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch4
 *
 * Description  : Set period of the GPT Channel 4.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setperiod_ch4(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[4], period, period_unit);
}
#endif

#if(GPT_CH5)
/******************************************************************************************
 * Function Name: gpt_drv_configure5
 *
 * Description  : Configure GPT Channel 5 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt5_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure5(void (*p_gpt5_event)(void))
{
	g_sim_gpts[5].timer_name = "gpt5_timer";
	g_sim_gpts[5].channel_no = 5;
#ifdef GPT5_CALLBACK
	g_sim_gpts[5].app_callback = p_gpt5_event;
#endif

	g_sim_gpts[5].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch5
 *
 * Description  : Open GPT Channel 5.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch5(void)
{
	return gpt_open_ch(&g_sim_gpts[5]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch5
 *
 * Description  : Close GPT Channel 5.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch5(void)
{
	return gpt_close_ch(&g_sim_gpts[5]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch5
 *
 * Description  : Start the GPT Channel 5.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch5(void)
{
	return gpt_start_ch(&g_sim_gpts[5]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch5
 *
 * Description  : Start the GPT Channel 5.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch5(void)
{
	return gpt_stop_ch(&g_sim_gpts[5]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch5
 *
 * Description  : Set duty of the GPT Channel 5.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setduty_ch5(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[5], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch5
 *
 * Description  : Set period of the GPT Channel 5.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setperiod_ch5(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[5], period, period_unit);
}
#endif

#if(GPT_CH6)
/******************************************************************************************
 * Function Name: gpt_drv_configure6
 *
 * Description  : Configure GPT Channel 6 Driver HAL. Call this method only once.
 *
 * Arguments    : *p_gpt6_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure6(void (*p_gpt6_event)(void))
{
	g_sim_gpts[6].timer_name = "gpt6_timer";
	g_sim_gpts[6].channel_no = 6;
#ifdef GPT6_CALLBACK
	g_sim_gpts[6].app_callback = p_gpt6_event;
#endif

	g_sim_gpts[6].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch6
 *
 * Description  : Open GPT Channel 6.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch6(void)
{
	return gpt_open_ch(&g_sim_gpts[6]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch6
 *
 * Description  : Close GPT Channel 6.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch6(void)
{
	return gpt_close_ch(&g_sim_gpts[6]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch6
 *
 * Description  : Start the GPT Channel 6.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch6(void)
{
	return gpt_start_ch(&g_sim_gpts[6]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch6
 *
 * Description  : Start the GPT Channel 6.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch6(void)
{
	return gpt_stop_ch(&g_sim_gpts[6]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch6
 *
 * Description  : Set duty of the GPT Channel 6.
 *
 * Arguments    : uint8_t pin
 *                uint8_t duty
 *
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setduty_ch6(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[6], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch6
 *
 * Description  : Set period of the GPT Channel 6.
 *
 * Arguments    : uint16_t period
 *                tmr_unit_t period_unit
 *
 * Return Value : bool
 *****************************************************************************************/
app_err_t gpt_setperiod_ch6(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[6], period, period_unit);
}
#endif

#if(GPT_CH7)
/******************************************************************************************
 * Function Name: gpt_drv_configure7
 *
 * Description  : Configure GPT Channel 7 Driver HAL. Call this method only once.
 *
 * Arguments    : void (*p_gpt7_event)(void)
 *
 * Return Value : None
 *******************************************************************************************/
void gpt_drv_configure7(void (*p_gpt7_event)(void))
{
	g_sim_gpts[7].timer_name = "gpt7_timer";
	g_sim_gpts[7].channel_no = 7;
#ifdef GPT7_CALLBACK
	g_sim_gpts[7].app_callback = p_gpt7_event;
#endif

	g_sim_gpts[7].is_configured = TRUE;
}

/******************************************************************************************
 * Function Name: gpt_open_ch7
 *
 * Description  : Open GPT Channel 7.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_open_ch7(void)
{
	return gpt_open_ch(&g_sim_gpts[7]);
}

/******************************************************************************************
 * Function Name: gpt_close_ch7
 *
 * Description  : Close GPT Channel 7.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_close_ch7(void)
{
	return gpt_close_ch(&g_sim_gpts[7]);
}

/******************************************************************************************
 * Function Name: gpt_start_ch7
 *
 * Description  : Start the GPT Channel 7.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_start_ch7(void)
{
	return gpt_start_ch(&g_sim_gpts[7]);
}

/******************************************************************************************
 * Function Name: gpt_stop_ch7
 *
 * Description  : Stop the GPT Channel 7.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_stop_ch7(void)
{
	return gpt_stop_ch(&g_sim_gpts[7]);
}

/******************************************************************************************
 * Function Name: gpt_setduty_ch7
 *
 * Description  : Set duty of the GPT Channel 7.
 *
 * Arguments    : uint8_t pin, uint8_t duty
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setduty_ch7(uint8_t pin, uint8_t duty)
{
	return gpt_setduty_ch(&g_sim_gpts[7], pin, duty);
}

/******************************************************************************************
 * Function Name: gpt_setperiod_ch7
 *
 * Description  : Set period of the GPT Channel 7.
 *
 * Arguments    : uint16_t period, tmr_unit_t period_unit
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *****************************************************************************************/
app_err_t gpt_setperiod_ch7(uint16_t period, app_tmr_unit_t period_unit)
{
	return gpt_setperiod_ch(&g_sim_gpts[7], period, period_unit);
}
#endif

/******************************************************************************************
 * Function Name: GPT_DATA_RECEIVE
 *
 * Description  : GPT general data receive.
 *
 * Arguments    : GptData* p_in_gpt_data
 *
 * Return Value : None
 ******************************************************************************************/
void GPT_DATA_RECEIVE(GptData* p_in_gpt_data)
{
	for (uint8_t data_index = 0; data_index < p_in_gpt_data->channel_count; data_index++)
	{
		switch (p_in_gpt_data->Channel[data_index])
		{
		case 0:
#if (GPT_CH0)
			g_sim_gpts[0].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[0], p_in_gpt_data->Period[0]);
			break;
#endif
		case 1:
#if (GPT_CH1)
			g_sim_gpts[1].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[1], p_in_gpt_data->Period[1]);
			break;
#endif
		case 2:
#if (GPT_CH2)
			g_sim_gpts[2].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[2], p_in_gpt_data->Period[2]);
			break;
#endif
		case 3:
#if (GPT_CH3)
			g_sim_gpts[3].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[3], p_in_gpt_data->Period[3]);
			break;
#endif
		case 4:
#if (GPT_CH4)
			g_sim_gpts[4].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[4], p_in_gpt_data->Period[4]);
			break;
#endif
		case 5:
#if (GPT_CH5)
			g_sim_gpts[5].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[5], p_in_gpt_data->Period[5]);
			break;
#endif
		case 6:
#if (GPT_CH6)
			g_sim_gpts[6].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[6], p_in_gpt_data->Period[6]);
			break;
#endif
		case 7:
#if (GPT_CH7)
			g_sim_gpts[7].initial_period_ms = _timeunit_to_ms(p_in_gpt_data->Unit[7], p_in_gpt_data->Period[7]);
			break;
#endif		
		default:
			break;
		}

	}
}
#endif
