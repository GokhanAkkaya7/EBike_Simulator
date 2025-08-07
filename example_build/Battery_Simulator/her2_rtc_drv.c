/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_rtc_drv.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : RTC Driver Module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 25.03.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_rtc_drv.h"

#if(RTC_DRV)
#include "driver_api.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

typedef struct
{
	bool is_configured;
	bool is_started;
	struct tm current_time;
	void (*user_periodic_callback)(void);
	TX_TIMER periodic_timer;      // This timer directly simulates the periodic IRQ.
	TX_TIMER one_second_timer;    // This timer is responsible for advancing the calendar time.
} sim_rtc_state_t;

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define RTC_MONTHS_IN_A_YEAR                (11)        // Month    : valid range between 0 to 11.
#define RTC_HOURS_IN_A_DAY                  (23)        // Hour     : valid range between 0 to 23.
#define RTC_MINUTES_IN_A_HOUR               (59)        // Minute   : valid range between 0 to 59.
#define RTC_SECONDS_IN_A_MINUTE             (59)        // Second   : valid range between 0 to 59.

#define RTC_DAY_VALUE_MIN                   (1)         // Lower bound of the RTC day value.
#define RTC_YEAR_VALUE_MIN                  (100)       // Lower bound of the RTC year value.
#define RTC_YEAR_VALUE_MAX                  (199)       // Upper bound of the RTC day value.

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX rtc_lock;
static sim_rtc_state_t g_rtc_state;
static Message response_message;

/*---------------------------------- Global Variables ------------------------------------*/

extern TX_QUEUE g_outgoing_message_queue;

/*------------------------------- Private Function Prototypes ----------------------------*/

static void one_second_tick_callback(ULONG input);
static void periodic_irq_callback(ULONG input);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: one_second_tick_callback
 *
 * Description  : This callback is triggered by a 1-second timer and is responsible
 *                for advancing the simulated calendar time by one second.
 *
 * Arguments    : ULONG input
 *
 * Return Value : None
 *****************************************************************************************/
static void one_second_tick_callback(ULONG input)
{
	(void)input; // Mark parameter as unused
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);

	g_rtc_state.current_time.tm_sec++;
	mktime(&g_rtc_state.current_time);

	tx_res = tx_mutex_put(&rtc_lock);
}

/******************************************************************************************
 * Function Name: periodic_irq_callback
 *
 * Description  : This callback directly simulates the RTC's periodic IRQ event. It calls
 *                the user-registered application callback.
 *
 * Arguments    : ULONG input
 *
 * Return Value : None
 *****************************************************************************************/
static void periodic_irq_callback(ULONG input)
{
	(void)input;
	if (g_rtc_state.user_periodic_callback != NULL)
	{
		g_rtc_state.user_periodic_callback();
	}
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: rtc_drv_configure
 *
 * Description  : Configure RTC Driver HAL. Call this method only once.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *******************************************************************************************/
app_err_t rtc_drv_configure(void (*p_rtc_event)(void))
{
	if (g_rtc_state.is_configured)
		return APP_SUCCESS;

	memset(&g_rtc_state, 0, sizeof(sim_rtc_state_t));

#if(RTC_CALLBACK)
	g_rtc_state.user_periodic_callback = p_rtc_event;
#else
	void(p_rtc_event);
#endif	

	UINT status = tx_mutex_create(&rtc_lock, (CHAR*)"RTC Ready Lock", TX_INHERIT);
	if (TX_SUCCESS != status)
		return APP_ERR_THREADX;

	if (TX_SUCCESS != tx_timer_create(&g_rtc_state.periodic_timer, "RTC Periodic IRQ",
		periodic_irq_callback, 0,
		(1 * MS_IN_SEC), (1 * MS_IN_SEC), TX_NO_ACTIVATE))
		return APP_ERR_THREADX;

	if (TX_SUCCESS != tx_timer_create(&g_rtc_state.one_second_timer, "RTC 1-Sec Tick",
		one_second_tick_callback, 0,
		(1 * MS_IN_SEC), (1 * MS_IN_SEC), TX_NO_ACTIVATE))
		return APP_ERR_THREADX;


	g_rtc_state.is_configured = true;
	return APP_SUCCESS;
}

/******************************************************************************************
 * Function Name: rtc_drv_init
 *
 * Description  : Initialize RTC module.
 *
 * Arguments    : void
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t rtc_drv_init(void)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
	{
		result = APP_ERR_DRV_CONFIGURE;
	}
	else
	{
		time_t current_time_val = mktime(&g_rtc_state.current_time);
		if (current_time_val == 0 || current_time_val == (time_t)-1)
		{
			// Set to a default time, e.g., Jan 1, 2025, 00:00:00
			g_rtc_state.current_time.tm_year = 2025 - 1900; // tm_year is years since 1900
			g_rtc_state.current_time.tm_mon = 0;             // 0 = January
			g_rtc_state.current_time.tm_mday = 1;
			g_rtc_state.current_time.tm_hour = 0;
			g_rtc_state.current_time.tm_min = 0;
			g_rtc_state.current_time.tm_sec = 0;
			mktime(&g_rtc_state.current_time); // Normalize and fill other fields like tm_wday
		}
	}

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: rtc_drv_start
 *
 * Description  : Start RTC module.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t rtc_drv_start(void)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else if (g_rtc_state.is_started)
		result = APP_SUCCESS;
	else
		if (TX_SUCCESS == tx_timer_activate(&g_rtc_state.one_second_timer))
			g_rtc_state.is_started = true;
		else
			result = APP_ERR_THREADX;

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: rtc_drv_deinit
 *
 * Description  : Close the RTC module.
 *
 * Arguments    : void
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t rtc_drv_deinit(void)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else
	{
		tx_timer_deactivate(&g_rtc_state.one_second_timer);
		tx_timer_deactivate(&g_rtc_state.periodic_timer);
		g_rtc_state.is_started = false;
	}

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: rtc_drv_set_irq
 *
 * Description  : Set periodic interrupt for RTC module with given period.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t rtc_drv_set_irq(rtc_period_t period)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else
	{
		uint32_t period_ticks = 0;

		switch (period)
		{
		case rtc_div_256_sec:
			period_ticks = (1 * MS_IN_SEC) / 256;		// ~3.9 -> 4 ticks
			break;
		case rtc_div_128_sec:
			period_ticks = (1 * MS_IN_SEC) / 128;		// ~7.8 -> 8 ticks
			break;
		case rtc_div_64_sec:
			period_ticks = (1 * MS_IN_SEC) / 64;		// ~15.6 -> 16 ticks 
			break;
		case rtc_div_32_sec:
			period_ticks = (1 * MS_IN_SEC) / 32;
			break;
		case rtc_div_16_sec:
			period_ticks = (1 * MS_IN_SEC) / 16;
			break;
		case rtc_div_8_sec:
			period_ticks = (1 * MS_IN_SEC) / 8;
			break;
		case rtc_div_4_sec:
			period_ticks = (1 * MS_IN_SEC) / 4;
			break;
		case rtc_div_2_sec:
			period_ticks = (1 * MS_IN_SEC) / 2;
			break;
		case rtc_1_sec:
			period_ticks = (1 * MS_IN_SEC);
			break;
		case rtc_2_sec:
			period_ticks = (2 * MS_IN_SEC);
			break;
		}

		if (period_ticks == 0) period_ticks = 1; // Ensure a minimum of 1 tick.

		tx_timer_deactivate(&g_rtc_state.periodic_timer);

		if (TX_SUCCESS != tx_timer_change(&g_rtc_state.periodic_timer, period_ticks, period_ticks))
		{
			result = APP_ERR_THREADX;
		}
		else
		{
			if (TX_SUCCESS != tx_timer_activate(&g_rtc_state.periodic_timer))
				result = APP_ERR_THREADX;
		}
	}

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: rtc_drv_time_set
 *
 * Description  : Sets internal RTC time with the given time.
 *
 * Arguments    : struct tm  *p_time
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *                APP_ERR_ASSERTION             Given parameters are not appropriate.
 ******************************************************************************************/
app_err_t rtc_drv_time_set(struct tm* p_time)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else if (p_time == NULL)
		result = APP_ERR_ASSERTION;
	else
	{
		g_rtc_state.current_time = *p_time;
		mktime(&g_rtc_state.current_time);		// Normalize the user-provided time
	}

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: rtc_drv_time_get
 *
 * Description  : Returns current time stored in RTC module.
 *
 * Arguments    : struct tm *p_time
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
app_err_t rtc_drv_time_get(struct tm* p_time)
{
	UINT tx_res = tx_mutex_get(&rtc_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_rtc_state.is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else if (p_time == NULL)
		result = APP_ERR_ASSERTION;
	else
		*p_time = g_rtc_state.current_time;

	tx_res = tx_mutex_put(&rtc_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

void RTC_DATAHANDLER(RtcData* in_rtc_data)
{
	if (in_rtc_data == NULL)
	{
		return;
	}
	struct tm new_time;

	switch (in_rtc_data->rtc_event)
	{
	case RTC_SET:

		new_time.tm_year = in_rtc_data->year - 1900;
		new_time.tm_mon = in_rtc_data->month - 1;
		new_time.tm_mday = in_rtc_data->day;
		new_time.tm_hour = in_rtc_data->hour;
		new_time.tm_min = in_rtc_data->minute;
		new_time.tm_sec = in_rtc_data->second;
		new_time.tm_isdst = -1;
		rtc_drv_time_set(&new_time);
		break;
	case RTC_GET:

		rtc_drv_time_get(&new_time);

		response_message.driver = DRIVER_RTC;
		response_message.data.rtc.rtc_event = RTC_SET;
		response_message.data.rtc.year = new_time.tm_year + 1900;
		response_message.data.rtc.month = new_time.tm_mon + 1;
		response_message.data.rtc.day = new_time.tm_mday;
		response_message.data.rtc.hour = new_time.tm_hour;
		response_message.data.rtc.minute = new_time.tm_min;
		response_message.data.rtc.second = new_time.tm_sec;

		Message* p_msg = &response_message;

		UINT status = tx_queue_send(&g_outgoing_message_queue, &p_msg, TX_NO_WAIT);
	default:
		break;
	}
}
#endif
