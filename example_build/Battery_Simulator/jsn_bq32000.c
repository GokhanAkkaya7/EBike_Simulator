/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_bq32000.c
* Author       : Gokhan Akkaya
* Description  : TI BQ32000 battery backup external RTC module.
* Project      : VESTEL Common modules project
* Creation Date: 02.04.2021
******************************************************************************************/

/*------------------------------------ Includes ------------------------------------------*/

#include "jsn_bq32000.h"

#if(BQ32000)
#include "conf_bq32000.h"
#include "driver_api.h"
#include "her2_rtc_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define TIME_SIZE                           7

/*------------------------------------ Static Variables ----------------------------------*/

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/***********************************************************************************************************************
 * Function Name: bq32000_init
 * Description  : Initialize BQ32000 External Real Time Clock.
 * Arguments    : None
 * Return Value : bool
 ***********************************************************************************************************************/
bool bq32000_init(void)
{
	if (APP_SUCCESS == rtc_drv_configure(NULL))
		if (APP_SUCCESS == rtc_drv_init())
			return true;

	return false;
}

/***********************************************************************************************************************
 * Function Name: bq32000_time_set
 * Description  : Converts the parameters to BCD format and sets the BQ32000 External Real Time Clock.
 * Arguments    : rtc_time_t *p_time
 *                sec -
 *                  seconds, 0-59
 *                min -
 *                  minutes, 0-59
 *                hour
 *                  hours,   0-24
 *                day -
 *                  day of the week, 1-Sunday, 2-Monday .... 7-Saturday
 *                date -
 *                  date,    1-31
 *                month -
 *                  month,   1-12
 *                year -
 *                  year,    0-255
 * Return Value : None
 ***********************************************************************************************************************/
bool bq32000_time_set(const struct tm* p_time, uint8_t rtry_count)
{
	(void)rtry_count;

	if (NULL == p_time)
		return false;

	RtcData ext_rtc_data =
	{ .rtc_event = RTC_SET,
		.second = p_time->tm_sec,
		.minute = p_time->tm_min,
		.hour = p_time->tm_hour,
		.day = p_time->tm_mday,
		.month = p_time->tm_mon + 1,
		.year = p_time->tm_year + 1900 };

	RTC_DATAHANDLER(&ext_rtc_data);

	return true;
}

/***********************************************************************************************************************
 * Function Name: bq32000_time_get
 * Description  : Reads the time and date from BQ32000 External Real Time Clock and Converts the BCD format.
 *                and set the RTC_Time current_time structure.
 * Arguments    : rtc_time_t *p_time
 * Return Value : bool
 * ***********************************************************************************************************************/
bool bq32000_time_get(struct tm* p_time, uint8_t rtry_count)
{
	(void)rtry_count;

	RtcData ext_rtc_data = { 0 };
	ext_rtc_data.rtc_event = RTC_GET;

	RTC_DATAHANDLER(&ext_rtc_data);

	memset(p_time, 0, sizeof(struct tm));
	p_time->tm_sec = ext_rtc_data.second;
	p_time->tm_min = ext_rtc_data.minute;
	p_time->tm_hour = ext_rtc_data.hour;
	p_time->tm_mday = ext_rtc_data.day;
	p_time->tm_mon = ext_rtc_data.month - 1;
	p_time->tm_year = ext_rtc_data.year - 1900;

	return true;
}

#endif
