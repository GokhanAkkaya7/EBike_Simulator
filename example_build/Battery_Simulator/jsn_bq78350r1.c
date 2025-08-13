/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_bq78350r1.c
 * Author       : Emre Erisgen
 * Description  : TI BQ78350-R1 gauge IC module.
 * Project      : VESTEL Common modules project
 * Creation Date: 19.04.2021
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_bq78350r1.h"
#if(BQ78350)

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define NO_WAIT_AFTER                       0       // For commands which do not require timeout period after.
#define FETEN_WAIT_AFTER                    75      // Wait 75ms after FET-EN command.
#define CONF_BLOCK_WAIT                     0

#define BQ78350R1_REGS_SETTING              1
#define BQ78350R2_REGS_SETTING              2
#define SERIAL_MODEL_NO_LENGTH				20

#define APPEND_TO_BUFFER(buffer, offset, value) \
    memcpy((buffer) + (offset), &(value), sizeof(value)); \
    (offset) += sizeof(value);

/*------------------------------ Private Typedef Definitions  ----------------------------*/

typedef struct
{
	uint8_t serial_num_data[BQ78350R1_MANU_INFO_BLOCK_SIZE];
}gauge_hw_data_t;

/*------------------------------------ Static Variables ----------------------------------*/

static uint16_t g_version = 1;
static gauge_hw_data_t gaughw_data = { 0 };

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: bq78350r1_init
 * Description  : Initialize BQ78350-R1 Gauge module.
 * Arguments    : none
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_init(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: get_firmware_version
 *
 * Description  : Read firmware version from BQ78350-R1 Gauge
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void bq78350_get_firmware_version(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = strnlen(bms_message.data.bms.firmware_version, MAX_BQ78350_RESP_SIZE);

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = bms_message.data.bms.firmware_version[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_rtcurr
 * Description  : Read Real Time Current(mA) from BQ78350-R1 Gauge current in milliAmpers.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_rtcurr(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.current;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: get_operation_status
 * Description  : Read Operation Status from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_opstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.operation_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.operation_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_chgstatus
 * Description  : This command returns the ChargingStatus() flags from BQ78350-R1 Gauge.
 *                15  14  13 12  11   10    9   8    7    6     5   4  3  2  1  0
 *                VCT RSVD SU IN RSVD RSVD FCHG PCHG RSVD RSVD RSVD OT HT ST LT UT
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_chgstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.charging_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.charging_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_gaugstatus
 * Description  : Read Gauging Status from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_gaugstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.gauging_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.gauging_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_packvolt
 * Description  : Read Battery Pack Voltage(mV) from BQ78350-R1 Gauge
 *                pack voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_packvolt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.pack_voltage;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell10volt
 * Description  : Read Cell 10(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell15volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[14];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell9volt
 * Description  : Read Cell 9(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell14volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[13];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell8volt
 * Description  : Read Cell 8(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell13volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[12];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell7volt
 * Description  : Read Cell 7(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell12volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[11];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell6volt
 * Description  : Read Cell 6(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell11volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[10];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell10volt
 * Description  : Read Cell 10(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell10volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[9];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell9volt
 * Description  : Read Cell 9(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell9volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[8];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell8volt
 * Description  : Read Cell 8(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell8volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[7];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell7volt
 * Description  : Read Cell 7(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell7volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[6];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell6volt
 * Description  : Read Cell 6(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell6volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[5];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell5volt
 * Description  : Read Cell 5(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell5volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[4];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell4volt
 * Description  : Read Cell 4(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell4volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[3];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell3volt
 * Description  : Read Cell 3(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell3volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[2];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell2volt
 * Description  : Read Cell 2(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell2volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[1];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cell1volt
 * Description  : Read Cell 1(mV) from BQ78350-R1 Gauge, value is cell voltage in milliVolts.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cell1volt(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cell_voltages[0];
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_battemp
 * Description  : Read Battery Temperature(0.1 Kelvin) from BQ78350-R1 Gauge
 *                pack temperature in (0.1 Kelvin) units
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_battemp(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.temperature;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_dastatus2s
 * Description  : Read DAStatus2 from BQ78350-R1 Gauge
 *                DAStatus2 data:
 *                   ExtAveCellVoltage,
 *                   VAUX Voltage,
 *                   TS1 Temperature,
 *                   TS2 Temperature,
 *                   TS3 Temperature,
 *                   Cell Temperature,
 *                   FET Temperature,
 *                   Internal Gauge Temperature
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *
 *******************************************************************************************/
bool bq78350r1_get_dastatus2(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->valuep[0] = 34;
		p_gauge_resp->valuep[1] = 0;
		p_gauge_resp->valuep[2] = 25;
		p_gauge_resp->valuep[3] = 25;
		p_gauge_resp->valuep[4] = 25;
		p_gauge_resp->valuep[5] = 25;
		p_gauge_resp->valuep[6] = 25;
		p_gauge_resp->valuep[7] = 25;
		p_gauge_resp->p_size = (uint8_t)8;
		p_gauge_resp->response = 1;

		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}

}

/******************************************************************************************
 * Function Name: bq78350r1_get_relsoc
 * Description  : Read Relative State of Charge(%) from BQ78350-R1 Gauge
 *                value is Relative State of Charge Percentage.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_relsoc(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.relative_soc;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cyclecount
 * Description  : Read Cycle Count from BQ78350-R1 Gauge
 *                value is Cycle Count Value.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cyclecount(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.cycle_count;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_soh
 * Description  : Read State of Health(%) from BQ78350-R1 Gauge, value is State of Health Percentage
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_soh(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.state_of_health;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_manstatus
 * Description  : Read Manufacturing Status from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_manstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.manufacturing_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.manufacturing_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_safstatus
 * Description  : This command returns the SafetyStatus() flags from BQ78350-R1 Gauge.
 *                31   30   29   28   27   26   25   24   23   22   21  20  19   18   17   16
 *                RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD OC RSVD CTO  RSVD PTO
 *                15   14   13    12     11 10   9   8    7    6     5     4   3   2   1   0
 *                RSVD OCDL OTF AFE_OVRD UTD UTC OTD OTC ASCDL ASCD AOLDL AOLD OCD OCC COV CUV
 *                RSVD (Bits 31–21): Reserved
 *                OC (Bit 20)  : Overcharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                RSVD (Bit 19): Reserved
 *                CTO (Bit 18) : Charge Timeout
 *                            1 = Detected
 *                            0 = Not Detected
 *                RSVD (Bit 17): Reserved
 *                PTO (Bit 16): Precharge Timeout
 *                            1 = Detected
 *                            0 = Not Detected
 *                RSVD (Bits 15): Reserved
 *                OCDL (Bit 14): Overcurrent During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTF (Bit 13): Overtemperature Fault
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFE_OVRD (Bit 12): AFE Alert
 *                            1 = Detected
 *                            0 = Not Detected
 *                UTD (Bit 11): Undertemperature During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                UTC (Bit 10): Undertemperature During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTD (Bit 9): Overtemperature During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTC (Bit 8): Overtemperature During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                ASCDL (Bit 7): Short Circuit During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                ASCD (Bit 6): Short Circuit During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLDL (Bit 5): Overload During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLD (Bit 4): Overload During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLD (Bit 4): Overload During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OCD (Bit 3): Overcurrent During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OCC (Bit 2): Overcurrent During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                COV (Bit 1): Cell Overvoltage
 *                            1 = Detected
 *                            0 = Not Detected
 *                CUV (Bit 0): Cell Undervoltage
 *                            1 = Detected
 *                            0 = Not Detected
 * Arguments    : bq78350r1_resp_t *smbus_res
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_safstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.safety_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.safety_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_pfstatus
 * Description  : This command returns the SafetyStatus() flags from BQ78350-R1 Gauge
 *        31   30   29   28   27   26   25   24   23   22   21   20  19    18   17   16
 *       RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD  DFW  IFC
 *        15   14   13  12    11         10   9     8    7      6     5   4   3   2     1   0
 *       SOTF TS3  TS3  TS2 AFE_XDRY AFE_OVRD AFEC AFER DFETF CFETF VIMR SOT SOCD SOCC SOV SUV
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_pfstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.pf_status);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.pf_status;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_remcap
 * Description  : Read Remaining Capacity(mAh) from BQ78350-R1 Gauge
 *                value is Remaining Capacity in milliAmper-hour.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_remcap(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.remaining_capacity;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_fullchgcap
 * Description  : Read Full Charge Capacity(mAh) from BQ78350-R1 Gauge
 *                value is Full Charge Capacity in milliAmper-hour.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_fullchgcap(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.full_charge_capacity;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_da_conf
 * Description  : Reads DA conf at 0x44AB from BQ78350-R1 Gauge
 *                   SLEEP,
 *                   IN_SYSTEM_SLEEP,
 *                   VAUX_SCALE,
 *                   VAUXEN,
 *                   ExtAveEN,
 *                   RSVD,
 *                   CTEMP,
 *                   FTEMP
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *
 *******************************************************************************************/
bool bq78350r1_get_da_conf(bq78350r1_resp_t* p_gauge_resp)
{

	p_gauge_resp->valuep[0] = 0;
	return true;

}

/******************************************************************************************
 * Function Name: bq78350r1_set_gpioctrl
 * Description  : This write-word function allows the host to program GPIO Outputs enabled
 *                in GPIO Type to a High or Low state. A Read of this command returns what was
 *                written to the command. value is 2 byte hexadecimal data.
 *                  7      6     5    4    3    2    1    0
 *                GPIO_B GPIO_A LED5 LED4 LED3 LED2 LED1 RSVD
 *                GPIO B (Bit 7): GPIO B
 *                        1 = High
 *                        0 = Low
 *                GPIO A (Bit 6): GPIO A
 *                        1 = High
 *                        0 = Low
 *                LED 5  (Bit 5): GPIO 5
 *                        1 = High
 *                        0 = Low
 *                LED 4  (Bit 4): GPIO 4
 *                        1 = High
 *                        0 = Low
 *                LED 3  (Bit 3): GPIO 3
 *                        1 = High
 *                        0 = Low
 *                LED 2  (Bit 2): GPIO 2
 *                        1 = High
 *                        0 = Low
 *                LED 1  (Bit 1): GPIO 1
 *                        1 = High
 *                        0 = Low
 *                RSVD (Bit 0): Reserved
 * Arguments    : bq78350r1_resp_t *p_gauge_resp, uint16_t *gpio_control_set
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_gpioctrl(uint16_t gpio_control_set)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_fetctrl
 * Description  : Setting fet on-off on each send. No command can be send for 75ms after this command.
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_fetctrl(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_shutdown
 * Description  : Shutdown Gauge for shipment mode.
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_shutdown(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_gauge_reset
 * Description  : Apply device reset to  gauge for gauge parameter updates.
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_gauge_reset(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_pf_clear
 * Description  : Apply device clear the pf status
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_pf_clear(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_sleepmode
 * Description  : Set sleep mode.
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_sleepmode(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_wakemode
 * Description  : Set set_wake_mode mode.
 * Arguments    : None
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_set_wakemode(void)
{
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_manifacturer_no
 * Description  : Sets the manufacturer no registers according to given data and data length and
 *                manipulates length field of manufacturer info accordingly.
 * Arguments    : uint8_t data_len : Length of the data to be written.
 *                uint8_t *p_serial_no_str : Pointer to the data to be written.
 * Return Value : bool : Success of the write operation
 *******************************************************************************************/
bool bq78350r1_set_manifacturer_no(uint8_t data_len, uint8_t* p_serial_no_str)
{
	if (BQ78350R1_MANU_INFO_BLOCK_SIZE < data_len)
		return false;

	gaughw_data.serial_num_data[0] = data_len;
	for (uint8_t inc = 0; inc < data_len; inc++)
		gaughw_data.serial_num_data[inc + 1] = p_serial_no_str[inc];

	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_set_curr_deadband_calib
 * Description  : Sets the current deadband in calibration register to change current deadband on field .
 * Arguments    : None.
 * Return Value : bool : Success of the write operation
 *******************************************************************************************/
bool bq78350r1_set_curr_deadband_calib(uint8_t current_deadband)
{
	(void)current_deadband;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_curr_deadband_calib
 * Description  : Gets the current deadband in calibration register.
 * Arguments    : uint8_t* current_deadband: Pointer to read current deadband value.
 * Return Value : bool : Success of the get operation
 *******************************************************************************************/
bool bq78350r1_get_curr_deadband_calib(uint8_t* current_deadband)
{
	(void)current_deadband;
	return true;
}

#if(GAUGE_ADMIN_MODE)
/******************************************************************************************
 * Function Name: bq78350_set_cuv_th
 * Description  : Set CUV threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cuv_th(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_UNDER_VOLTAGE_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_UNDER_VOLTAGE_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cuv_rec
 * Description  : Set CUV recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cuv_rec(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_UNDER_VOLTAGE_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_UNDER_VOLTAGE_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_th
 * Description  : Set COV threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_th(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_rec
 * Description  : Set COV recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_rec(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

#if(BQ78350_VERSION_R2)
/******************************************************************************************
 * Function Name: bq78350_set_cov_th
 * Description  : Set COV Low threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_low_th(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_LOW_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_LOW_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_rec
 * Description  : Set COV Low recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_low_rec(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_LOW_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_LOW_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_high_th
 * Description  : Set COV High threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_high_th(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_HIGH_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_HIGH_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_high_rec
 * Description  : Set COV High recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_high_rec(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_HIGH_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_HIGH_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_recom_th
 * Description  : Set COV Recom threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_recom_th(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_RECOM_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_RECOM_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_cov_high_rec
 * Description  : Set COV REcom recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_cov_recom_rec(uint16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_RECOM_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_VOLTAGE_RECOM_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}
#endif

/******************************************************************************************
 * Function Name: bq78350_set_utd_th
 * Description  : Set UTD threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_utd_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->UNDER_TEMP_DISCH_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->UNDER_TEMP_DISCH_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_utd_rec
 * Description  : Set UTD recovery value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_utd_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->UNDER_TEMP_DISCH_TRESH_REC_REG & 0xff),
	  (uint8_t)(active_reg->UNDER_TEMP_DISCH_TRESH_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_utc_th
 * Description  : Set UTC threshold value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_utc_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->UNDER_TEMP_CHARGE_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->UNDER_TEMP_CHARGE_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_utc_rec
 * Description  : Set UTC RECOVERY value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_utc_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->UNDER_TEMP_CHARGE_TRESH_REC_REG & 0xff),
	  (uint8_t)(active_reg->UNDER_TEMP_CHARGE_TRESH_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otd_th
 * Description  : Set OTD THRESHOLD value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otd_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_TEMP_DISCH_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->OVER_TEMP_DISCH_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otd_rec
 * Description  : Set OTD RECOVERY value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otd_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_TEMP_DISCH_REC_REG & 0xff),
	  (uint8_t)(active_reg->OVER_TEMP_DISCH_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otf_th
 * Description  : Set OTF THRESHOLD value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otf_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_FET_TEMP_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->OVER_FET_TEMP_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otf_rec
 * Description  : Set OTF RECOVERY value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otf_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_FET_TEMP_REC_REG & 0xff),
	  (uint8_t)(active_reg->OVER_FET_TEMP_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_ocd_th
 * Description  : Set OCD THRESHOLD value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_ocd_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->DISCH_OVER_CURR_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->DISCH_OVER_CURR_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_ocd_rec
 * Description  : Set OCD RECOVERY value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_ocd_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->DISCH_OVER_CURR_REC_REG & 0xff),
	  (uint8_t)(active_reg->DISCH_OVER_CURR_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_occ_th
 * Description  : Set OCC THRESHOLD value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_occ_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_CURR_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_CURR_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_occ_rec
 * Description  : Set OCC RECOVERY value
 * Arguments    : uint16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_occ_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->CHARGE_OVER_CURR_REC_REG & 0xff),
	  (uint8_t)(active_reg->CHARGE_OVER_CURR_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otc_th
 * Description  : Set OtC THRESHOLD value
 * Arguments    : int16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otc_th(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_TEMP_CHARGE_TRESH_REG & 0xff),
	  (uint8_t)(active_reg->OVER_TEMP_CHARGE_TRESH_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_set_otc_rec
 * Description  : Set OtC RECOVERY value
 * Arguments    : int16_t *
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_set_otc_rec(int16_t* p_value)
{
	uint8_t wake_arr[4] =
	{ (uint8_t)(active_reg->OVER_TEMP_CHARGE_REC_REG & 0xff),
	  (uint8_t)(active_reg->OVER_TEMP_CHARGE_REC_REG >> 8),
	  *((uint8_t*)p_value + 1),
	  *((uint8_t*)p_value) };

	return smbus_write_block(active_reg->SLEEP_WAKE_REG, wake_arr, BQ78350R1_CUV_TH_CMD_SIZE, NO_WAIT_AFTER);
}

/******************************************************************************************
 * Function Name: bq78350_import_gauge
 * Description  : writes the specified values to the addresses given for Gauge Import
 * Arguments    : uint8_t buff[]
 * Return Value : bool
 *******************************************************************************************/
bool bq78350_import_gauge(uint8_t buff[GAUGE_IMPORT_BUFFER_LENGTH])
{
	return smbus_write_block(active_reg->SLEEP_WAKE_REG, buff, GAUGE_IMPORT_BUFFER_LENGTH, NO_WAIT_AFTER);
}
#endif

/******************************************************************************************
 * Function Name: bq78350r1_get_abssoc
 * Description  : Read Absolute State of Charge(%) from BQ78350-R1 Gauge
 *                value is Absolute State of Charge Percentage
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_abssoc(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.relative_soc;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_pendedv
 * Description  : Read the predicted EDV2 until EDV2 is reached, then the predicted EDV1 until
 *                EDV1 is reached, and then the predicted EDV0
 *                value is pending EDV in milliVolts
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_pendedv(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.design_voltage;  // TODO GA: it is not design voltage.
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_avgtimeempty
 * Description  : Read predicted remaining battery capacity based on AverageCurrent()
 *                from BQ78350-R1 Gauge.
 *                value is average time to empty in minutes , if value is 65535 then battery
 *                is not discharging
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_avgtimeempty(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		int16_t current_mA = bms_message.data.bms.current;
		uint16_t remaining_capacity_mAh = bms_message.data.bms.remaining_capacity;

		if (-10 <= current_mA)
		{
			p_gauge_resp->value = 65535;
		}
		else
		{
			float discharge_current_mA = (float)fabs(current_mA);
			float time_in_hours = (float)remaining_capacity_mAh / discharge_current_mA;
			float time_in_minutes = time_in_hours * 60.0f;
			p_gauge_resp->value = (uint16_t)round(time_in_minutes);
			p_gauge_resp->response = 1;
		}
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_avgtimefull
 * Description  : Read predicted time to full charge based on AverageCurrent().
 *                from BQ78350-R1 Gauge.
 *                value is average time to full in minutes , if value is 65535 then battery
 *                is not discharging.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_avgtimefull(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		int16_t current_mA = bms_message.data.bms.current;
		uint16_t remaining_capacity_mAh = bms_message.data.bms.remaining_capacity;
		uint16_t full_charge_capacity_mAh = bms_message.data.bms.full_charge_capacity;

		if (10 > current_mA)
		{
			p_gauge_resp->value = 65535;
		}
		else if (remaining_capacity_mAh >= full_charge_capacity_mAh)
		{
			p_gauge_resp->value = 0;
		}
		else
		{
			float needed_capacity_mAh = (float)(full_charge_capacity_mAh - remaining_capacity_mAh);
			float charge_current_mA = (float)current_mA;
			float time_in_hours = needed_capacity_mAh / charge_current_mA;
			float time_in_minutes = time_in_hours * 60.0f;

			p_gauge_resp->value = (uint16_t)round(time_in_minutes);
			p_gauge_resp->response = 1;
		}
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_chemicalid
 * Description  : Read chemical id from BQ78350-R1 Gauge
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_chemicalid(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (DRIVER_BMS == bms_message.driver)
	{
		size_t msg_len = sizeof(bms_message.data.bms.chemical_id);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.chemical_id;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: get_manufacturer_name
 * Description  : Read Manufacturer Name from BQ78350-R1 Gauge
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_manufacturer_name(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (DRIVER_BMS == bms_message.driver)
	{
		size_t msg_len = strnlen(bms_message.data.bms.manufacturer_name, MAX_BQ78350_RESP_SIZE);

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = bms_message.data.bms.manufacturer_name[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_mandate
 * Description  : Read Manufacturer Date from BQ78350-R1 Gauge and the value is 2-byte
 *                manufacturer date.
 *                The ManufacturerDate() value is in the following format:
 *                Day + Month � 32 + (Year�1980) � 512, and is stored in Manufacturer Date.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_mandate(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.manufacture_date;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_designcap
 * Description  : Read Design Capacity(mAh) from BQ78350-R1 Gauge
 *                value is Design Capacity in milliAmper-hours
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_designcap(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.remaining_capacity;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_batstatus
 * Description  : This read-word function returns various battery status information
 *                from BQ78350-R1 Gauge
 *                value is 2-byte battery status.
 *                15  14  13   12  11  10   9   8   7    6  5  4   3   2   1   0
 *                OCA TCA RSVD OTA TDA RSVD RCA RTA INIT DSG FC FD EC3 EC2 EC1 EC0
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_batstatus(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		p_gauge_resp->value = bms_message.data.bms.battery_status;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime1
 * Description  : Read Lifetime Data Block 1 from BQ78350-R1 Gauge
 *                lifetime block 1 data is the max cell voltages (1-15). 32 + 1 bytes.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime1(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{

		size_t data_size = sizeof(bms_message.data.bms.lifetime_max_cell_v); // 15 * 2 = 30 bytes

		memcpy(p_gauge_resp->valuep, bms_message.data.bms.lifetime_max_cell_v, data_size);

		p_gauge_resp->p_size = (uint8_t)data_size;
		p_gauge_resp->response = 1;
		return true;
	}

	p_gauge_resp->response = 0;
	return false;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime2
 * Description  : Read Lifetime Data Block 2 from BQ78350-R1 Gauge
 *                lifetime block 2 data is the min cell voltages (1-15). 32 + 1 bytes.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime2(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{

		size_t data_size = sizeof(bms_message.data.bms.lifetime_min_cell_v); // 15 * 2 = 30 bytes

		memcpy(p_gauge_resp->valuep, bms_message.data.bms.lifetime_min_cell_v, data_size);

		p_gauge_resp->p_size = (uint8_t)data_size;
		p_gauge_resp->response = 1;
		return true;
	}

	p_gauge_resp->response = 0;
	return false;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime3
 * Description  : Read Lifetime Data Block 3 from BQ78350-R1 Gauge
 *                lifetime block 3 data is 14 + 1 bytes.
 *                max delta cell voltage,
 *                max chg current,
 *                max dsg current,
 *                max avg dsg current,
 *                max avg dsg power,
 *                max cell temp,
 *                min cell temp,
 *                max delta temp cell,
 *                max fet temp,
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime3(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);
	if (bms_message.driver != DRIVER_BMS)
	{
		p_gauge_resp->response = 0;
		return false;
	}

	uint8_t offset = 0;
	BmsData* bms = &bms_message.data.bms;

	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_delta_cell_v);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_charge_current);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_discharge_current);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_avg_dsg_current);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_avg_dsg_power);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_temp_cell);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_min_temp_cell);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_delta_temp_cell);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_max_temp_fet);

	p_gauge_resp->p_size = offset;
	p_gauge_resp->response = 1;
	return true;

	//bugfix for Max Cell Temp, Min Cell Temp, Max Delta Temp Cell and Max FET Temp
	//order of the values do not mismatch with BQ78350-R1 techical reference manual
	//swap = p_gauge_resp->valuep[10];
	//p_gauge_resp->valuep[10] = p_gauge_resp->valuep[11];
	//p_gauge_resp->valuep[11] = swap;

	//swap = p_gauge_resp->valuep[12];
	//p_gauge_resp->valuep[12] = p_gauge_resp->valuep[13];
	//p_gauge_resp->valuep[13] = swap;

}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime4
 * Description  : Read Lifetime Data Block 4 from BQ78350-R1 Gauge
 *                lifetime block 4 data is 20 + 1 bytes
 *                no of shutdowns,
 *                reserved,
 *                reserved,
 *                reserved,
 *                Cell Balance Time Cell 1,
 *                Cell Balance Time Cell 2,
 *                Cell Balance Time Cell 3,
 *                ***,
 *                ***,
 *                Cell Balance Time Cell 15,
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime4(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);
	if (bms_message.driver != DRIVER_BMS) { p_gauge_resp->response = 0; return false; }

	uint8_t offset = 0;
	BmsData* bms = &bms_message.data.bms;
	uint16_t reserved = 0; // Reserved bytes must be 0

	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_shutdowns_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, reserved);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, reserved);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, reserved);

	memcpy(p_gauge_resp->valuep + offset, bms->lifetime_cell_balance_time, sizeof(bms->lifetime_cell_balance_time));
	offset += sizeof(bms->lifetime_cell_balance_time);

	p_gauge_resp->p_size = offset;
	p_gauge_resp->response = 1;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime5
 * Description  : Read Lifetime Data Block 5 from BQ78350-R1 Gauge
 *                lifetime block 5 data is 14 + 1 bytes
 *                Total Fw Runtime,
 *                Time Spent in UT,
 *                Time Spent in LT,
 *                Time Spent in ST,
 *                Time Spent in HT,
 *                Time Spent in OT,
 *                Time Since Last Charge,
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime5(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);
	if (bms_message.driver != DRIVER_BMS) { p_gauge_resp->response = 0; return false; }

	uint8_t offset = 0;
	BmsData* bms = &bms_message.data.bms;
	uint16_t time_since_charge = 0; // Example, can be made dynamic

	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_total_fw_runtime_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_time_spent_ut_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_time_spent_lt_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_time_spent_st_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_time_spent_ht_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_time_spent_ot_days);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, time_since_charge);

	p_gauge_resp->p_size = offset;
	p_gauge_resp->response = 1;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime6
 * Description  : Read Lifetime Data Block 6 from BQ78350-R1 Gauge
 *                lifetime block 6 data is 32 + 1 bytes.
 *                No of COV Events,
 *                Last COV Event,
 *                No of CUV Events,
 *                Last CUV Event,
 *                No of OCD Events,
 *                Last OCD Event,
 *                No of OCC Events,
 *                Last OCC Event,
 *                No of AOLD Events,
 *                Last AOLD Event,
 *                No of ASCD Events,
 *                Last ASCD Event,
 *                No of OTC Events,
 *                Last OTC Event,
 *                No of OTD Events,
 *                Last OTD Event,
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime6(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);
	if (bms_message.driver != DRIVER_BMS) { p_gauge_resp->response = 0; return false; }

	uint8_t offset = 0;
	BmsData* bms = &bms_message.data.bms;
	uint16_t last_event_time = 0; // Example, can be made dynamic

	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_cov_events_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_cuv_events_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_ocd_events_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_occ_events_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	// ... Append other event counts (AOLD, ASCD, OTC, OTD) here ...

	p_gauge_resp->p_size = offset; // Update size based on how many you add
	p_gauge_resp->response = 1;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_lifetime7
 * Description  : Read Lifetime Data Block 7 from BQ78350-R1 Gauge
 *                lifetime block 7 data:
 *                    No of OTF Events,
 *                    Last OTF Event,
 *                    No of Valid Charge Terminations,
 *                    Last Valid Charge Termination,
 *                    No of FCC Update Events,
 *                    Last FCC Update,
 *                    No of Shutdowns,
 *                    Reserved,
 *                    Reserved,
 *                    Reserved,
 *                    CB Time Cell 1,
 *                    CB Time Cell 2,
 *                    ..,
 *                    ..,
 *                    ..,
 *                    CB Time Cell 15, 32 + 1 bytes.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_lifetime7(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);
	if (bms_message.driver != DRIVER_BMS) { p_gauge_resp->response = 0; return false; }

	uint8_t offset = 0;
	BmsData* bms = &bms_message.data.bms;
	uint16_t last_event_time = 0; 

	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_otf_events_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_valid_charge_term_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, bms->lifetime_fcc_update_count);
	APPEND_TO_BUFFER(p_gauge_resp->valuep, offset, last_event_time);

	p_gauge_resp->p_size = offset;
	p_gauge_resp->response = 1;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_maninfo
 * Description  : Read Manufacturer Info from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_maninfo(bq78350r1_resp_t* p_gauge_resp)
{
	size_t data_len = sizeof(gaughw_data.serial_num_data);
	for (uint8_t inc = 0; inc < data_len; inc++)
		p_gauge_resp->valuep[inc] = gaughw_data.serial_num_data[inc];

	p_gauge_resp->p_size = (uint8_t) data_len;
	p_gauge_resp->response = 1;
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_devname
 * Description  : Read Device Name Info from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_devname(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = strnlen(bms_message.data.bms.device_name, MAX_BQ78350_RESP_SIZE);

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = bms_message.data.bms.device_name[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_otd_th
 * Description  : Read otd threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_otd_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_otd_rec_th
 * Description  : Read otd recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_otd_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_oft_th
 * Description  : Read oft threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_oft_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_oft_rec_th
 * Description  : Read oft recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_oft_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_doc_th
 * Description  : Read doc threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_doc_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_doc_rec_th
 * Description  : Read doc recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_doc_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_uvt_th
 * Description  : Read uvt threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_uvt_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_uvt_rec_th
 * Description  : Read uvt recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_uvt_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_ctl_th
 * Description  : Read ctl threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_ctl_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_ctl_rec_th
 * Description  : Read ctl recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_ctl_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_dtl_th
 * Description  : Read dtl threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_dtl_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_dtl_rec_th
 * Description  : Read dtl recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_dtl_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cov_rec_th
 * Description  : Read cov threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cov_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cov_rec_th
 * Description  : Read cov recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cov_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.cov_recovery);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.cov_recovery;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cuv_th
 * Description  : Read cov threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cuv_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_cuv_rec_th
 * Description  : Read cov_rec threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_cuv_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	Message bms_message = { 0 };
	get_main_data(&bms_message, DRIVER_BMS);

	if (bms_message.driver == DRIVER_BMS)
	{
		size_t msg_len = sizeof(bms_message.data.bms.cuv_recovery);
		uint8_t* p_value = (uint8_t*)&bms_message.data.bms.cuv_recovery;

		for (uint8_t inc = 0; inc < msg_len; inc++)
			p_gauge_resp->valuep[inc] = p_value[inc];

		p_gauge_resp->p_size = (uint8_t)msg_len;
		p_gauge_resp->response = 1;
		return true;
	}
	else
	{
		p_gauge_resp->p_size = 0;
		p_gauge_resp->response = 0;
		return false;
	}
}

/******************************************************************************************
 * Function Name: bq78350r1_get_occ_th
 * Description  : Read occ threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_occ_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_occ_rec_th
 * Description  : Read occ recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_occ_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_otc_th
 * Description  : Read otc threshold from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_otc_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_otc_rec_th
 * Description  : Read otc recovery from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 * Return Value : bool
 *******************************************************************************************/
bool bq78350r1_get_otc_rec_th(bq78350r1_resp_t* p_gauge_resp)
{
	//TODO GA: May be later if needed.
	return true;
}

/******************************************************************************************
 * Function Name: bq78350r1_get_df_block
 * Description  : Read specified dataflash block from BQ78350-R1 Gauge.
 * Arguments    : bq78350r1_resp_t *p_gauge_resp - Pointer to structured buffer
 *                uint8_t block_size - Size of the block to be read
 *                uint16_t block_reg - Adress of to block to be read
 * Return Value : bool  -Status of the  bq78350r1_get_th_block function
 *******************************************************************************************/
bool bq78350r1_get_df_block(bq78350r1_resp_t* p_gauge_resp, uint8_t block_size, uint16_t block_reg)
{
	//TODO GA: May be later.
	return true;
}

#if(UNUSED_COMMANDS)
/******************************************************************************************
 * Function Name: get_dynamic_power
 *
 * Description  : The bq78350-R1 includes the read-only DynamicPower() word command, which
 *                returns a value of Current() × Voltage() in 10 mW units. If Current() and/or
 *                Voltage() are scaled, then the units of DynamicPower() are also scaled.
 *
 *                value is 2-byte integer in 10mW units
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_dynamic_power(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t data;

	if (smbus_read_reg_wtimeout(active_reg->DYNAMIC_POWER_REG, &data) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = data;
	}
}

/******************************************************************************************
 * Function Name: get_ext_ave_cell_voltage
 *
 * Description  : This read-word function returns the external average cell voltage measurement,
 *                if enabled, which can be used within the gas gauging algorithm for EDV2, EDV1,
 *                and EDV0 detection
 *
 *                value is 2-byte unsigned integer in mV units
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_ext_ave_cell_voltage(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t data;

	if (smbus_read_reg_wtimeout(active_reg->EXTAVECELL_VOLTAGE_REG, &data) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = data;
	}
}

/******************************************************************************************
 * Function Name: get_safety_alert
 *
 * Description  : This command returns the SafetyAlert() flags from BQ78350-R1 Gauge
 *
 *                31   30   29   28   27   26   25   24   23   22   21  20  19   18   17   16
 *                RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD RSVD OC CTOS RSVD PTOS RSVD
 *
 *                15   14   13    12     11 10   9   8    7    6     5     4   3   2   1   0
 *                RSVD OCDL OTF AFE_OVRD UTD UTC OTD OTC ASCDL ASCD AOLDL AOLD OCD OCC COV CUV
 *
 *                RSVD (Bits 31–21): Reserved
 *                OC (Bit 20): Overcharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                CTOS (Bit 19): Charge Timeout Suspend
 *                            1 = Detected
 *                            0 = Not Detected
 *                RSVD (Bit 18): Reserved
 *                PTOS (Bit 17): Precharge Timeout Suspend
 *                            1 = Detected
 *                            0 = Not Detected
 *                RSVD (Bits 16–15): Reserved
 *                OCDL (Bit 14): Overcurrent During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTF (Bit 13): Overtemperature Fault
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFE_OVRD (Bit 12): AFE Alert
 *                            1 = Detected
 *                            0 = Not Detected
 *                UTD (Bit 11): Undertemperature During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                UTC (Bit 10): Undertemperature During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTD (Bit 9): Overtemperature During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OTC (Bit 8): Overtemperature During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                ASCDL (Bit 7): Short Circuit During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                ASCD (Bit 6): Short Circuit During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLDL (Bit 5): Overload During Discharge Latch
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLD (Bit 4): Overload During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                AOLD (Bit 4): Overload During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OCD (Bit 3): Overcurrent During Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                OCC (Bit 2): Overcurrent During Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                COV (Bit 1): Cell Overvoltage
 *                            1 = Detected
 *                            0 = Not Detected
 *                CUV (Bit 0): Cell Undervoltage
 *                            1 = Detected
 *                            0 = Not Detected
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_safety_alert(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t safety_alert[BQ78350R1_SAFETYALERT_BLOCK_SIZE], i = 0;
	uint16_t safetyalert_reg = 0x0050;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &safetyalert_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, safety_alert,
			BQ78350R1_SAFETYALERT_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < safety_alert[0]; i++)
				p_gauge_resp->valuep[i] = safety_alert[i + 1];
			p_gauge_resp->p_size = safety_alert[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: GetPFAlert
 * Description  : This command returns the PFAlert() flags from BQ78350-R1 Gauge
 *
 *          15   14  13  12    11       10     9    8     7     6    5    4   3     2   1   0
 *         SOTF TS3 TS2 TS1 AFE_XRDY AFE_OVRD AFEC AFER DFETF CFETF VIMR SOT SOCD SOCC SOV SUV
 *
 *                SOTF (Bit 15): Safety Overtemperature FET Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                TS3 (Bit 14): Open Thermistor – TS3 Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                TS2 (Bit 13): Open Thermistor – TS2 Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                TS1 (Bit 12): Open Thermistor – TS1 Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFE_XRDY (Bit 11): Companion bq769x0 AFE XREADY Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFE_OVRD (Bit 10): Companion bq769x0 AFE Override Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFEC (Bit 9): AFE Communication Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                AFER (Bit 8): AFE Register Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                DFETF (Bit 7): Discharge FET Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                CFETF (Bit 6): Charge FET Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                VIMR (Bit 5): Voltage Imbalance While Pack Is at Rest Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                SOT (Bit 4): Safety Overtemperature Cell Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                SOCD (Bit 3): Safety Overcurrent in Discharge
 *                            1 = Detected
 *                            0 = Not Detected
 *                SOCC (Bit 2): Safety Overcurrent in Charge
 *                            1 = Detected
 *                            0 = Not Detected
 *                SOV (Bit 1): Safety Cell Overvoltage Failure
 *                            1 = Detected
 *                            0 = Not Detected
 *                SUV (Bit 0): Safety Cell Undervoltage Failure
 *                            1 = Detected
 *                            0 = Not Detected
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_pf_alert(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t pf_alert[BQ78350R1_PFALERT_BLOCK_SIZE], i = 0;
	uint16_t pfalert_reg = 0x0052;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &pfalert_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, pf_alert, BQ78350R1_PFALERT_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < pf_alert[0]; i++)
				p_gauge_resp->valuep[i] = pf_alert[i + 1];
			p_gauge_resp->p_size = pf_alert[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_afe_status
 *
 * Description  : This command returns 2-byte hexadecimal configuration of the companion AFE
 *                See the AFE bq769x0 3-Series to 15-Series Cell Battery Monitor Family for
 *                Li-Ion and Phosphate Applications Data Manual (SLUSBK2) for further details.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_afe_status(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t afe_status[BQ78350R1_AFE_STATUS_BLOCK_SIZE], i = 0;
	uint16_t afe_status_reg = 0x0058;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &afe_status_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, afe_status,
			BQ78350R1_AFE_STATUS_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < afe_status[0]; i++)
				p_gauge_resp->valuep[i] = afe_status[i + 1];
			p_gauge_resp->p_size = afe_status[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_afe_config
 *
 * Description  : This command returns the 11+1 byte string, which is configuration of the
 *                companion AFE. See the AFE bq769x0 3-Series to 15-Series Cell Battery Monitor
 *                Family for Li-Ion and Phosphate Applications Data Manual (SLUSBK2).
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_afe_config(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t afe_config[BQ78350R1_AFE_CONFIG_BLOCK_SIZE], i = 0;
	uint16_t afe_config_reg = 0x0059;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &afe_config_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, afe_config,
			BQ78350R1_AFE_CONFIG_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < afe_config[0]; i++)
				p_gauge_resp->valuep[i] = afe_config[i + 1];
			p_gauge_resp->p_size = afe_config[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_afe_vcx
 *
 * Description  : This command returns the 32+1 string, which is the cell voltage measurement
 *                data of the companion AFE. See the AFE bq769x0 3-Series to 15-Series Cell
 *                Battery Monitor Family for Li-Ion and Phosphate Applications Data Manual
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_afe_vcx(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t afe_vcx[BQ78350R1_AFE_VCX_BLOCK_SIZE], i = 0;
	uint16_t afe_vcx_reg = 0x005A;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &afe_vcx_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, afe_vcx, BQ78350R1_AFE_VCX_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < afe_vcx[0]; i++)
				p_gauge_resp->valuep[i] = afe_vcx[i + 1];
			p_gauge_resp->p_size = afe_vcx[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_afe_data
 *
 * Description  : This command returns 13+1 string, which includes the system voltage, temperature,
 *                and current measurement data of the companion AFE. See the AFE bq769x0 3-Series to
 *                15-Series Cell Battery Monitor Family for Li-Ion and Phosphate Applications Data Manual.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_afe_data(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t afe_data[BQ78350R1_AFE_DATA_BLOCK_SIZE], i = 0;
	uint16_t afe_data_reg = 0x005B;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &afe_data_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, afe_data,
			BQ78350R1_AFE_DATA_BLOCK_SIZE) == true)
		{

			p_gauge_resp->response = 1;
			for (i = 0; i < afe_data[0]; i++)
				p_gauge_resp->valuep[i] = afe_data[i + 1];
			p_gauge_resp->p_size = afe_data[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_da_status_1
 * Description  : Read DAStatus1 from BQ78350-R1 Gauge
 *
 *                Outputs 32 bytes of data on ManufacturerBlockAccess() or ManufacturerData() in the following format:
 *                aaAAbbBBccCCddDDeeEEffFFggGGhhHHiiIIjjJJkkKKllLLmmMMnnNNooOOppPP where:
 *                AAaa: Cell Voltage 1
 *                BBbb: Cell Voltage 2
 *                CCcc: Cell Voltage 3
 *                DDdd: Cell Voltage 4
 *                EEee: Cell Voltage 5
 *                FFff: Cell Voltage 6
 *                GGgg: Cell Voltage 7
 *                HHhh: Cell Voltage 8
 *                IIii: Cell Voltage 9
 *                JJjj: Cell Voltage 10
 *                KKkk: Cell Voltage 11
 *                LLll: Cell Voltage 12
 *                MMmm: Cell Voltage 13
 *                NNnn: Cell Voltage 14
 *                OOoo: Cell Voltage 15
 *                PPpp: Reserved
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_dastatus1(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t da_status1[BQ78350R1_DASTATUS1_BLOCK_SIZE], i = 0;
	uint16_t da_status1_reg = 0x0071;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &da_status1_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, da_status1,
			BQ78350R1_DASTATUS1_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < da_status1[0]; i++)
				p_gauge_resp->valuep[i] = da_status1[i + 1];
			p_gauge_resp->p_size = da_status1[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_cuv_snapshot
 * Description  : This command returns the CellVoltages on ManufacturerBlockAccess() or
 *                ManufacturerData() at the time SafetyStatus() [CUV] is set. It is refreshed
 *                each time [CUV] becomes set.
 *
 *                Outputs 32 bytes of data on ManufacturerBlockAccess() or ManufacturerData() in the following format:
 *                aaAAbbBBccCCddDDeeEEffFFggGGhhHHiiIIjjJJkkKKllLLmmMMnnNNooOOppPP where:
 *                AAaa: Cell Voltage 1
 *                BBbb: Cell Voltage 2
 *                CCcc: Cell Voltage 3
 *                DDdd: Cell Voltage 4
 *                EEee: Cell Voltage 5
 *                FFff: Cell Voltage 6
 *                GGgg: Cell Voltage 7
 *                HHhh: Cell Voltage 8
 *                IIii: Cell Voltage 9
 *                JJjj: Cell Voltage 10
 *                KKkk: Cell Voltage 11
 *                LLll: Cell Voltage 12
 *                MMmm: Cell Voltage 13
 *                NNnn: Cell Voltage 14
 *                OOoo: Cell Voltage 15
 *                PPpp: Reserved
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_cuv_snapshot(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t cuv_snapshot[BQ78350R1_CUVSNAPSHOT_BLOCK_SIZE], i = 0;
	uint16_t cuvsnap_reg = 0x0080;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &cuvsnap_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, cuv_snapshot,
			BQ78350R1_CUVSNAPSHOT_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < cuv_snapshot[0]; i++)
				p_gauge_resp->valuep[i] = cuv_snapshot[i + 1];
			p_gauge_resp->p_size = cuv_snapshot[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_cov_snapshot
 * Description  : This command returns the CellVoltages on ManufacturerBlockAccess() or
 *                ManufacturerData() at the time SafetyStatus() [COV] is set. It is refreshed
 *                each time [COV] becomes set.
 *
 *                Outputs 32 bytes of data on ManufacturerBlockAccess() or ManufacturerData() in the following format:
 *                aaAAbbBBccCCddDDeeEEffFFggGGhhHHiiIIjjJJkkKKllLLmmMMnnNNooOOppPP where:
 *                AAaa: Cell Voltage 1
 *                BBbb: Cell Voltage 2
 *                CCcc: Cell Voltage 3
 *                DDdd: Cell Voltage 4
 *                EEee: Cell Voltage 5
 *                FFff: Cell Voltage 6
 *                GGgg: Cell Voltage 7
 *                HHhh: Cell Voltage 8
 *                IIii: Cell Voltage 9
 *                JJjj: Cell Voltage 10
 *                KKkk: Cell Voltage 11
 *                LLll: Cell Voltage 12
 *                MMmm: Cell Voltage 13
 *                NNnn: Cell Voltage 14
 *                OOoo: Cell Voltage 15
 *                PPpp: Reserved
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_cov_snapshot(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t cov_snapshot[BQ78350R1_COVSNAPSHOT_BLOCK_SIZE], i = 0;
	uint16_t covsnap_reg = 0x0081;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &covsnap_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, cov_snapshot,
			BQ78350R1_COVSNAPSHOT_BLOCK_SIZE) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < cov_snapshot[0]; i++)
				p_gauge_resp->valuep[i] = cov_snapshot[i + 1];
			p_gauge_resp->p_size = cov_snapshot[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: get_hardware_version
 *
 * Description  : Read hardware version from BQ78350-R1 Gauge
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_hardware_version(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t hw_ver[20], i = 0;
	uint16_t hw_ver_reg = 0x0003;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &hw_ver_reg) == true)
	{
		if (smbus_read_block_wtimeout(active_reg->MANUFACTURER_ACC_BLOCK, hw_ver, 20) == true)
		{
			p_gauge_resp->response = 1;
			for (i = 0; i < hw_ver[0]; i++)
				p_gauge_resp->valuep[i] = hw_ver[i + 1];
			p_gauge_resp->p_size = hw_ver[0];
		}
		else
			p_gauge_resp->response = 0;
	}
	else
		p_gauge_resp->response = 0;
}

/******************************************************************************************
 * Function Name: set_life_time_data_reset
 * Description  : This command resets Lifetime data in data flash for ease of manufacturing.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *
 *******************************************************************************************/
void set_life_time_data_reset(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t lifetimereset_reg = 0x0028;
	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &lifetimereset_reg) == true)
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = 1;
	}
	else
	{
		p_gauge_resp->response = 0;
		p_gauge_resp->value = 0;
	}
}

/******************************************************************************************
 * Function Name: set_permanent_fail_data_reset
 * Description  : This command resets PF data in data flash for ease of manufacturing.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *
 *******************************************************************************************/
void set_permanent_fail_data_reset(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t pfreset_reg = 0x0029;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &pfreset_reg) == true)
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = 1;
	}
	else
	{
		p_gauge_resp->response = 0;
		p_gauge_resp->value = 0;
	}
}

/******************************************************************************************
 * Function Name: set_device_reset
 * Description  : This command resets the device.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *
 *******************************************************************************************/
void set_device_reset(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t devicereset_reg = 0x0012;

	if (smbus_write_reg_wtimeout(active_reg->MANUFACTURER_ACCESS_REG, &devicereset_reg) == true)
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = 1;
	}
	else
	{
		p_gauge_resp->response = 0;
		p_gauge_resp->value = 0;
	}
}

/******************************************************************************************
 * Function Name: get_remaining_capacity_alarm
 * Description  : Read a low-capacity alarm threshold value from BQ78350-R1 Gauge,
 *                default -> 300
 *                min     ->   0
 *                max     -> 700
 *                If RemainingCapacityAlarm() is set to 0, the alarm is disabled
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_remaining_capacity_alarm(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t remaining_cap_alarm;

	if (smbus_read_reg_wtimeout(active_reg->REMAINING_CAP_ALARM_REG, &remaining_cap_alarm) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = remaining_cap_alarm;
	}
}

/******************************************************************************************
 * Function Name: get_remaining_time_alarm
 * Description  : Read the RemainingTimeAlarm() value from BQ78350-R1 Gauge,
 *                default -> 10
 *                min     ->  0
 *                max     -> 30
 *                If RemainingTimeAlarm() is set to 0, the alarm is disabled
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/

void get_remaining_time_alarm(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t remaining_time_alarm;

	if (smbus_read_reg_wtimeout(active_reg->REMAINING_TIME_ALARM_REG, &remaining_time_alarm) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = remaining_time_alarm;
	}
}

/******************************************************************************************
 * Function Name: get_battery_mode
 * Description  : Read the battery's capabilities and modes from BQ78350-R1 Gauge,
 *
 *                min     -> 0x0000
 *                max     -> 0xFFFF
 *
 *           15  14  13  12   11   10   9   8  7   6   5    4    3    2    1   0
 *          CAPM CHGM AM RSVD RSVD RSVD PB CC CF RSVD RSVD RSVD RSVD RSVD PBS ICC
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_battery_mode(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t battery_mode;
	if (smbus_read_reg_wtimeout(active_reg->BATTERY_MODE_REG, &battery_mode) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = battery_mode;
	}
}

/******************************************************************************************
 * Function Name: get_at_rate
 * Description  : This read function is the first half of a two-function call
 *                set used to set the AtRate value, which is used in calculations made by the
 *                AtRateTimeToFull(), AtRateTimeToEmpty(), and AtRateOK() functions.
 *
 *                the atRate value
 *                is in (mAh)    units if BatteryMode()[CAPM] = 0  or
 *                is in (10 mWh) units if BatteryMode()[CAPM] = 1
 *
 *                min     -> -32768
 *                max     ->  32767
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *
 *******************************************************************************************/
void get_at_rate(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t at_rate;
	if (smbus_read_reg_wtimeout(active_reg->ATRATE_REG, &at_rate) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = at_rate;
	}
}

/******************************************************************************************
 * Function Name: get_at_rate_time_to_full
 * Description  : This read function returns an unsigned integer value of the predicted
 *                remaining time to fully charge the battery using a CC-CV method at the AtRate()
 *                value in minutes with a range of 0 to 65,534
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_at_rate_time_to_full(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t at_rate_time_to_full;
	if (smbus_read_reg_wtimeout(active_reg->ATRATE_TIMETOFULL_REG, &at_rate_time_to_full) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = at_rate_time_to_full;
	}
}

/******************************************************************************************
 * Function Name: get_at_rate_time_to_empty
 * Description  : This read-word function returns an unsigned integer value of the predicted
 *                remaining operating time in minutes with a range of 0 to 65,534 if the battery
 *                is discharged at the AtRate() value.
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *
 *******************************************************************************************/
void get_at_rate_time_to_empty(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t at_rate_time_to_empty;

	if (smbus_read_reg_wtimeout(active_reg->ATRATE_TIMETOEMPTY_REG, &at_rate_time_to_empty) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = at_rate_time_to_empty;
	}
}

/******************************************************************************************
 * Function Name: get_at_rate_ok
 * Description  : This read-word function returns a boolean value that indicates whether or
 *                not the battery can deliver the AtRate() value of energy for 10 seconds.
 *                The bq78350-R1 updates this value within 1 s after the SMBus host sets the
 *                AtRate() function value. The bq78350-R1 updates AtRateOK() at 1-s intervals.
 *
 *                value is 0 for No, 1 for Yes.
 *
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_at_rate_ok(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t at_rate_ok;

	if (smbus_read_reg_wtimeout(active_reg->ATRATE_OK_REG, &at_rate_ok) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = at_rate_ok;
	}
}

/******************************************************************************************
 * Function Name: get_avarage_current
 * Description  : This read-word function returns a signed integer value that approximates a
 *                one-minute rolling average of the current being supplied (or accepted) through
 *                the battery terminals in mA, with a range of –32,768 to 32,767.
 *
 *                value is avg current in milliAmpers
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_avarage_current(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t avg_current; // Change Signed/Unsigned
	if (smbus_read_reg_wtimeout(active_reg->BATTERY_AVG_CURRENT_REG, &avg_current) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = avg_current;
	}
}

/******************************************************************************************
 * Function Name: get_max_error
 * Description  : This read-word function returns the expected margin of error, in %, in the
 *                state-of-charge calculation with a range of 1 to 100%.
 *
 *                value is the expected margin of error, in %
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_max_error(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t max_error;
	if (smbus_read_reg_wtimeout(active_reg->BATTERY_MAXERROR_REG, &max_error) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = max_error;
	}
}

/******************************************************************************************
 * Function Name: get_run_time_empty
 *
 * Description  : Read predicted remaining battery capacity (minutes) based on the present rate of
 *                discharge from BQ78350-R1 Gauge
 *
 *                value is run time to empty in minutes, if value is 65535 then battery is not discharging
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_run_time_empty(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t data;
	if (smbus_read_reg_wtimeout(active_reg->RUNTIMETO_EMPTY_REG, &data) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = data;
	}
}

/******************************************************************************************
 * Function Name: get_charging_current
 *
 * Description  : This read-word function returns the desired charging current
 *                from BQ78350-R1 Gauge
 *
 *                value is charging current in mA
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_charging_current(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t data;
	if (smbus_read_reg_wtimeout(active_reg->CHARGING_CURR_REG, &data) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = data;
	}
}

/******************************************************************************************
 * Function Name: get_charging_voltage
 *
 * Description  : This read-word function returns the desired charging voltage
 *                from BQ78350-R1 Gauge
 *
 *                value is charging voltage in mV
 *
 * Arguments    : bq78350r1_resp_t *smbus_res
 *
 * Return Value : none
 *******************************************************************************************/
void get_charging_voltage(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t data;
	if (smbus_read_reg_wtimeout(active_reg->CHARGING_VOLT_REG, &data) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = data;
	}
}

/******************************************************************************************
 * Function Name: get_design_voltage
 *
 * Description  : Read Design Voltage(mV) from BQ78350-R1 Gauge, value is Design Voltage in
 *                milliVolts
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_design_voltage(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t design_vol;

	if (smbus_read_reg_wtimeout(active_reg->DESIGN_VOLTAGE_REG, &design_vol) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = design_vol;
	}
}

/******************************************************************************************
 * Function Name: get_specification_info
 *
 * Description  : This read-word function returns, as an unsigned integer value, the version
 *                number of the Smart Battery Specification that the battery pack supports,
 *                as well as voltage- and current-scaling information.
 *                Default -> 0x0031
 *
 *                value is 2-byte Specification Info
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_specification_info(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t spec_info;

	if (smbus_read_reg_wtimeout(active_reg->SPEC_INFO_REG, &spec_info) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = spec_info;
	}
}

/******************************************************************************************
 * Function Name: get_device_chemistry
 *
 * Description  : Read Device Name from BQ78350-R1 Gauge
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_device_chemistry(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t device_chemistry[BQ78350R1_DEVICE_CHEM_BLOCK_SIZE], i = 0;

	if (smbus_read_block_wtimeout(active_reg->DEVICE_CHEM_REG, device_chemistry,
		BQ78350R1_DEVICE_CHEM_BLOCK_SIZE) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		for (i = 0; i < device_chemistry[0]; i++)
			p_gauge_resp->valuep[i] = device_chemistry[i + 1];

		//respond array's first element is the size
		p_gauge_resp->p_size = device_chemistry[0];
	}
}

/******************************************************************************************
 * Function Name: get_gpio_status
 *
 * Description  : This read-word function returns the bit-wise status of all the GPIO enabled
 *                in GPIO Config, value is 2 byte hexadecimal data
 *
 *                  7      6     5    4    3    2    1    0
 *                GPIO_B GPIO_A LED5 LED4 LED3 LED2 LED1 RSVD
 *
 *                GPIO B (Bit 7): GPIO B
 *                        1 = High
 *                        0 = Low
 *                GPIO A (Bit 6): GPIO A
 *                        1 = High
 *                        0 = Low
 *                LED 5  (Bit 5): GPIO 5
 *                        1 = High
 *                        0 = Low
 *                LED 4  (Bit 4): GPIO 4
 *                        1 = High
 *                        0 = Low
 *                LED 3  (Bit 3): GPIO 3
 *                        1 = High
 *                        0 = Low
 *                LED 2  (Bit 2): GPIO 2
 *                        1 = High
 *                        0 = Low
 *                LED 1  (Bit 1): GPIO 1
 *                        1 = High
 *                        0 = Low
 *                RSVD (Bit 0): Reserved
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_gpio_status(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t gpio_status;

	if (smbus_read_reg_wtimeout(active_reg->GPIO_STATUS_REG, &gpio_status) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = gpio_status;
	}
}

/******************************************************************************************
 * Function Name: get_vaux_voltage
 *
 * Description  : This read-word function returns an unsigned integer value representing the
 *                scaled measured voltage from the VAUX pin, in units of mV if [VAUX_SCALE] = 0,
 *                and 10 mV if [VAUX_SCALE] = 1 with a range of 0 to 65535.
 *                The returned value = VAUX × VAUX Gain.
 *
 *                value is 2 byte unsigned integer with units of mV
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_vaux_voltage(bq78350r1_resp_t* p_gauge_resp)
{
	uint16_t vaux_voltage;

	if (smbus_read_reg_wtimeout(active_reg->VAUX_VOLTAGE_REG, &vaux_voltage) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		p_gauge_resp->value = vaux_voltage;
	}
}

/******************************************************************************************
 * Function Name: get_authenticate
 *
 * Description  : This read block function provides SHA-1 authentication in the default mode
 *
 * Arguments    : bq78350r1_resp_t *p_gauge_resp
 *
 * Return Value : none
 *******************************************************************************************/
void get_authenticate(bq78350r1_resp_t* p_gauge_resp)
{
	static uint8_t authenticate[BQ78350R1_AUTHENTICATE_BLOCK_SIZE], i = 0;

	if (smbus_read_block_wtimeout(active_reg->AUTHENTICATE_REG, authenticate, BQ78350R1_AUTHENTICATE_BLOCK_SIZE) == false)
	{
		p_gauge_resp->response = 0;
	}
	else
	{
		p_gauge_resp->response = 1;
		for (i = 0; i < authenticate[0]; i++)
			p_gauge_resp->valuep[i] = authenticate[i + 1];

		//respond array's first element is the size
		p_gauge_resp->p_size = authenticate[0];
	}
}
#endif

#endif
