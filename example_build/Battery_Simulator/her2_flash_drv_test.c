/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_flash_drv_test.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Test application of Flash driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 15.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_flash_drv_test.h"

#if(CFLASH_TEST || DFLASH_TEST)
#include "her2_flash_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define DFLASH_CURR_BLOCK                   7
#define DFLASH_TEST_DATA_ADDRESS            (FLASH_DF_BLOCK_START + (DATAFLASH_BLOCK_SIZE *(DFLASH_CURR_BLOCK-1)))

#define DFLASH_TEST_ARR_SIZE                36

#define CFLASH_CURR_BLOCK                   100
#define CFLASH_TEST_DATA_ADDRESS            (FLASH_CF_BLOCK_START + (CODEFLASH_BLOCK_SIZE *(CFLASH_CURR_BLOCK-1)))

#define CFLASH_TEST_ARR_SIZE                200

/*------------------------------------ Static Variables ----------------------------------*/

#if (DFLASH_TEST)
static volatile bool dflash_test_res = false;
#endif

#if(CFLASH_TEST)
static volatile bool cflash_test_res = false;
#endif

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: flash_test_init
 *
 * Description  : Initialize application layer of the Flash module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
bool flash_test_init(void)
{
	if (APP_SUCCESS == flash_drv_configure())
		if (APP_SUCCESS == flash_drv_init())
			return true;

	return false;
}

#if(DFLASH_TEST)
/******************************************************************************************
 * Function Name: flash_test_main
 *
 * Description  : Test method of data flash module. Do basic operations to test if data flash
 *                is working as expected.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void flash_test_main(void)
{
	dflash_test_res = false;

	uint8_t* test_arr = "FLASHWRITE TEST FOR SIMULATOR TRIAL";
	uint8_t rtest_arr[DFLASH_TEST_ARR_SIZE];

	for (uint8_t indx = 0; indx < DFLASH_TEST_ARR_SIZE; indx++)
		rtest_arr[indx] = 0;


	if (APP_SUCCESS == dflash_drv_block_write(test_arr, DFLASH_TEST_DATA_ADDRESS, DFLASH_TEST_ARR_SIZE))
		if (APP_SUCCESS == dflash_drv_block_read(rtest_arr, DFLASH_TEST_DATA_ADDRESS, DFLASH_TEST_ARR_SIZE))
			if (rtest_arr[0] == test_arr[0] && rtest_arr[15] == test_arr[15] && rtest_arr[35] == test_arr[35])
				dflash_test_res = true;

}
#endif

#if (CFLASH_TEST)
/******************************************************************************************
 * Function Name: cflash_app_test
 *
 * Description  : Test method of Code flash module. Do basic operations to test if Code flash
 *                is working as expected.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void cflash_app_test(void)
{
	cflash_test_res = false;

	uint8_t test_arr[CFLASH_TEST_ARR_SIZE];
	uint8_t rtest_arr[CFLASH_TEST_ARR_SIZE];

	for (uint16_t indx = 0; indx < CFLASH_TEST_ARR_SIZE; indx++)
	{
		test_arr[indx] = (uint8_t)indx;
		rtest_arr[indx] = 0;
	}

	/* !!! Careful about the address you wanted to write. Be aware of writing an address you already run
	 *  the code to avoid overwriting. At the time of this test sequence written, 57 KB ( < 0x0001_0000 )
	 *  Code flash area used so the given address for test (100th block of 2KB blocks which means 0x0003_1800)
	 *  is pretty okay to test. !!!*/

	bool b_cf_is_blank = false;
	if (APP_SUCCESS == cflash_drv_block_blank_check(CFLASH_TEST_DATA_ADDRESS, &b_cf_is_blank))
	{
		if (b_cf_is_blank)
		{
			if (APP_SUCCESS == cflash_drv_block_write(test_arr, CFLASH_TEST_DATA_ADDRESS, CFLASH_TEST_ARR_SIZE))
				if (APP_SUCCESS == cflash_drv_block_read(rtest_arr, CFLASH_TEST_DATA_ADDRESS, CFLASH_TEST_ARR_SIZE))
					if (rtest_arr[0] == test_arr[0] && rtest_arr[35] == test_arr[35] && rtest_arr[110] == test_arr[110])
						if (APP_SUCCESS == cflash_drv_block_erase(CFLASH_TEST_DATA_ADDRESS))
							if (APP_SUCCESS == cflash_drv_block_blank_check(CFLASH_TEST_DATA_ADDRESS, &b_cf_is_blank))
								if (b_cf_is_blank)
									cflash_test_res = true;

		}
		else
		{
			if (APP_SUCCESS == cflash_drv_block_erase(CFLASH_TEST_DATA_ADDRESS))
				if (APP_SUCCESS == cflash_drv_block_read(rtest_arr, CFLASH_TEST_DATA_ADDRESS, CFLASH_TEST_ARR_SIZE))
					if (APP_SUCCESS == cflash_drv_block_blank_check(CFLASH_TEST_DATA_ADDRESS, &b_cf_is_blank))
						if (b_cf_is_blank)
							cflash_test_res = true;
		}
	}
}
#endif
#endif
