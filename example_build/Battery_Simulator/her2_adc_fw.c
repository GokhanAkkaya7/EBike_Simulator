/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_adc_fw.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Environment  : Microsoft Visual Studio 2022, Version 17.5.3.
 * Description  : ADC Microsoft Layer.
 * Project      : VESTEL Renesas Microsoft Layer.
 * Creation Date: 29.05.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_adc_fw.h"

#if(ADC_FW)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

static void (*adc0_event)(uint8_t, uint16_t*);
static TX_MUTEX adc_mutex;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

app_err_t adc0_fw_configure(void (*p_adc_event)(uint8_t, uint16_t*))
{
	adc0_event = p_adc_event;

	tx_mutex_create(&adc_mutex, "adc_mutex is created", TX_INHERIT);
}

app_err_t adc0_fw_open(void)
{
	UINT tx_res = tx_mutex_get(&adc_mutex, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}

	// TODO GA;

	tx_mutex_put(&adc_mutex);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}
}

app_err_t adc0_fw_start(void)
{
	UINT tx_res = tx_mutex_get(&adc_mutex, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}

	// TODO GA;

	tx_mutex_put(&adc_mutex);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}
}

app_err_t adc0_fw_stop(void)
{
	UINT tx_res = tx_mutex_get(&adc_mutex, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}

	// TODO GA;

	tx_mutex_put(&adc_mutex);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}
}

app_err_t adc0_fw_close(void)
{
	UINT tx_res = tx_mutex_get(&adc_mutex, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}

	// TODO GA;

	tx_mutex_put(&adc_mutex);
	if (TX_SUCCESS != tx_res)
	{
		printf("adc0_fw_open: unexpected error happened on ThreadX side. Error code: %d", tx_res);
		return APP_ERR_THREADX;
	}
}

/******************************************************************************************
 * Function Name: ADC0_FW_CALLBACK
 *
 * Description  : ADC Framework Callback Function for channel 0.
 *
 * Arguments    : uint8_t index, uint16_t* adc_buffer
 *
 * Return Value : None
 ******************************************************************************************/
void ADC0_FW_CALLBACK(uint8_t index, uint16_t* adc_buffer)
{
	adc0_event(index, adc_buffer);
}

#endif