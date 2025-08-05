/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_rtc_drv_test.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Test application of RTC driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 25.03.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_rtc_drv_test.h"

#if(RTC_TEST)
#include "her2_rtc_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

static volatile bool rtc_test_res = false;
static volatile uint16_t rtc_periodic_cnt = 0;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

static void rtc_event_periodic(void);

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: rtc_event_periodic
 *
 * Description  : Handles the incoming RTC driver periodic event.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
static void rtc_event_periodic(void)
{
    rtc_periodic_cnt++;
}

/******************************************************************************************
 * Function Name: rtc_app_init
 *
 * Description  : Initialize application layer of the RTC module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
bool rtc_test_init(void)
{
    if (APP_SUCCESS == rtc_drv_configure(&rtc_event_periodic))
        if (APP_SUCCESS == rtc_drv_init())
            if (APP_SUCCESS == rtc_drv_start())
                if (APP_SUCCESS == rtc_drv_set_irq(rtc_1_sec))
                    return true;

    return false;
}

/******************************************************************************************
 * Function Name: rtc_app_test
 *
 * Description  : Test method of RTC module. Do basic operations to test if RTC
 *                is working as expected.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void rtc_test_main(void)
{
    //rtc_test_res = false;
    //struct tm cur_time;

    //struct tm set_time =
    //{ .tm_sec = 7, .tm_min = 27, .tm_hour = 8, .tm_mday = 28, .tm_wday = 3, .tm_mon = 0, .tm_year = 100, };

    //if (APP_SUCCESS == rtc_drv_time_get(&cur_time))
    //    if (APP_SUCCESS == rtc_drv_time_set(&set_time))
    //        if (APP_SUCCESS == rtc_drv_time_get(&cur_time))
    //            rtc_test_res = true;

}

#endif
