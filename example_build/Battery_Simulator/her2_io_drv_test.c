/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_io_drv_test.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Test application of IO Driver module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 09.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_io_drv_test.h"

#if(IO_TEST)
#include "her2_io_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define TEST_PIN1                           APP_IO_PORT_03_PIN_04

/*------------------------------------ Static Variables ----------------------------------*/

static volatile bool io_test_res = false;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: io_app_init
 *
 * Description  : Initialize application layer of the IO Port module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
bool io_test_init(void)
{
    if (APP_SUCCESS == io_init())
        return true;
    return false;
}

/******************************************************************************************
 * Function Name: io_app_test
 * Description  : Test method of IO module.
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void io_test_main(void)
{
    app_io_level_t test_pin = APP_IO_LEVEL_LOW;
    io_test_res = false;

    if (APP_SUCCESS == get_pin_level(TEST_PIN1, &test_pin))
        if (APP_SUCCESS == set_pin_level(TEST_PIN1, APP_IO_LEVEL_HIGH))
            if (APP_SUCCESS == get_pin_level(TEST_PIN1, &test_pin))
                if (APP_IO_LEVEL_HIGH == test_pin)
                    io_test_res = true;
}
#endif
