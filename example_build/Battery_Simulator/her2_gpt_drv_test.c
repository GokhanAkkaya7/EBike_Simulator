/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_gpt_drv_test.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Test application of GPT driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 11.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_gpt_drv_test.h"

#if(GPT_TEST)
#include "her2_gpt_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define BLUE_LED                            0           // GTIOCA of the GPT CH4 (Make it "1" for GTIOCB)
#define LED_DUTY_PERCENT                    70          // 70% duty cycle.
#define COUNTER_PERIOD_IN_MS                2000        // 2000 msec -> 2 seconds.

/*------------------------------------ Static Variables ----------------------------------*/

static volatile bool gpt_test_res = false;
static volatile uint8_t counter_interrupt = 0;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

void gpt_callback_event(void);

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: gpt_callback_event
 *
 * Description  : Fired each time timer overflow occurs.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void gpt_callback_event(void)
{
    counter_interrupt++;
}

/******************************************************************************************
 * Function Name: gpt_app_init
 *
 * Description  : Initialize application layer of the GPT module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
bool gpt_test_init(void)
{
    if (APP_SUCCESS == gpt_init())
    {
        gpt_drv_configure1(gpt_callback_event);
        gpt_drv_configure4(NULL);
        if ((APP_SUCCESS == gpt_open_ch1()) && (APP_SUCCESS == gpt_open_ch4()))
            return true;
    }

    return false;
}

/******************************************************************************************
 * Function Name: gpt_app_test
 *
 * Description  : Test method of GPT module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
void gpt_test_main(void)
{
    gpt_test_res = false;

    if ((APP_SUCCESS == gpt_start_ch1()) && (APP_SUCCESS == gpt_start_ch4()))
        if ((APP_SUCCESS == gpt_setperiod_ch1(COUNTER_PERIOD_IN_MS, TMR_UNIT_PERIOD_MSEC))
            && (APP_SUCCESS == gpt_setduty_ch4(BLUE_LED, LED_DUTY_PERCENT)))
            gpt_test_res = true;

}
#endif
