#pragma once

#include "conf_project_settings.h"
#include "config_simulator.h"

typedef enum
{
	RTC_SET = 0, RTC_GET
} rtc_event_t;

typedef enum
{
	APP_IO_LEVEL_LOW = 0, APP_IO_LEVEL_HIGH
} app_io_level_t;

typedef enum
{
	Unknown_unit = 0,
	Seconds,
	Milliseconds,
	Microseconds,
	Hertz
}GptUnit_e;

typedef enum
{
	DRIVER_ADC = 0,
	DRIVER_UART,
	DRIVER_IO,
	DRIVER_IRQ,
	DRIVER_CAN,
	DRIVER_GPT,
	DRIVER_RTC,
	DRIVER_BMS,
	DRIVER_UNKNOWN
} DriverType_e;

typedef struct
{
	int index;
	uint16_t buffer[MAX_INCOME_DATA_BUFFER_SIZE];
	size_t buffer_len;
} AdcData;

typedef struct
{
	char value[MAX_INCOME_DATA_BUFFER_SIZE];
} UartData;

typedef struct
{
	uint16_t pin;
	app_io_level_t state; // 0 or 1
} IoData;

typedef struct
{
	int channel_count;
	uint16_t channel_list[MAX_INCOME_DATA_BUFFER_SIZE];

} IrqData;

typedef struct
{
	uint32_t id;
	int dlc;
	uint16_t can_buffer[8];
} CanData;

typedef struct
{
	int channel_count;
	int Channel[12];
	int Period[12];
	GptUnit_e Unit[12];

} GptData;

typedef struct
{
	rtc_event_t rtc_event;
	int second;
	int minute;
	int hour;
	int day;
	int month;
	int year;

} RtcData;

typedef struct
{
	// =======================================================================
	// CATEGORY 1: REAL-TIME SBS COMMANDS (Standard Battery System Commands)
	// These are the most frequently polled values for basic operation.
	// =======================================================================
	int16_t  current;               // Real-time Current (mA)
	uint16_t pack_voltage;          // Pack Voltage (mV)
	uint16_t temperature;           // Temperature (0.1K)
	uint16_t cell_voltages[15];     // Individual Cell Voltages (mV)
	uint8_t  relative_soc;          // Relative State of Charge (%)
	uint8_t  state_of_health;       // State of Health (%)
	uint16_t remaining_capacity;    // Remaining Capacity (mAh)
	uint16_t full_charge_capacity;  // Full Charge Capacity (mAh)
	uint16_t average_time_to_empty; // Average Time to Empty (minutes)
	uint16_t average_time_to_full;  // Average Time to Full (minutes)
	uint16_t cycle_count;           // Charge/Discharge Cycle Count

	// =======================================================================
	// CATEGORY 2: STATUS BLOCKS (Bit-mapped flags)
	// These blocks provide detailed state information.
	// =======================================================================
	uint32_t safety_status;         // SafetyStatus() flags
	uint16_t charging_status;       // ChargingStatus() flags
	uint32_t operation_status;      // OperationStatus() flags (FET status, etc.)
	uint16_t gauging_status;        // GaugingStatus() flags (charge termination, etc.)
	uint16_t battery_status;        // BatteryStatus() flags (alarms, etc.)
	uint16_t manufacturing_status;  // ManufacturingStatus() flags
	uint32_t pf_status;             // Permanent Failure Status flags

	// =======================================================================
	// CATEGORY 3: STRING AND INFO BLOCKS
	// Static or slowly changing information about the battery pack.
	// =======================================================================
	char     manufacturer_name[MAX_BQ78350_RESP_SIZE];
	char     device_name[MAX_BQ78350_RESP_SIZE];
	char     firmware_version[MAX_BQ78350_RESP_SIZE];
	uint16_t design_capacity;       // Design Capacity (mAh)
	uint16_t design_voltage;        // Design Voltage (mV)
	uint16_t manufacture_date;      // Encoded date
	uint16_t chemical_id;           // Chemical ID of the cells

	// =======================================================================
	// CATEGORY 4: CONFIGURATION DATA (DataFlash Thresholds)
	// These are protection limits and settings.
	// =======================================================================
	uint16_t cuv_threshold;
	uint16_t cuv_recovery;
	uint16_t cov_threshold;
	uint16_t cov_recovery;
	int16_t  occ_threshold;
	int16_t  occ_recovery;
	int16_t  ocd_threshold;
	int16_t  ocd_recovery;
	int16_t  utc_threshold; // In 0.1 Kelvin
	int16_t  utc_recovery;
	int16_t  otc_threshold;
	int16_t  otc_recovery;
	int16_t  utd_threshold;
	int16_t  utd_recovery;
	int16_t  otd_threshold;
	int16_t  otd_recovery;
	int16_t  otf_threshold;
	int16_t  otf_recovery;

	// =======================================================================
	// 5: LIFETIME DATA.
	// =======================================================================
	// 
	// Lifetime Block 1
	uint16_t lifetime_max_cell_v[15];
	// Lifetime Block 2
	uint16_t lifetime_min_cell_v[15];

	// Lifetime Block 3
	uint16_t lifetime_max_delta_cell_v;
	int16_t  lifetime_max_charge_current;
	int16_t  lifetime_max_discharge_current;
	int16_t  lifetime_max_avg_dsg_current;
	int16_t  lifetime_max_avg_dsg_power;
	int16_t  lifetime_max_temp_cell;
	int16_t  lifetime_min_temp_cell;
	uint16_t lifetime_max_delta_temp_cell;
	int16_t  lifetime_max_temp_fet;

	// Lifetime Block 4
	uint16_t lifetime_shutdowns_count;
	uint8_t  lifetime_cell_balance_time[15]; // Dakika cinsinden

	// Lifetime Block 5
	uint16_t lifetime_total_fw_runtime_days;
	uint16_t lifetime_time_spent_ut_days; // Under Temp
	uint16_t lifetime_time_spent_lt_days; // Low Temp
	uint16_t lifetime_time_spent_st_days; // Standard Temp
	uint16_t lifetime_time_spent_ht_days; // High Temp
	uint16_t lifetime_time_spent_ot_days; // Over Temp

	// Lifetime Block 6
	uint16_t lifetime_cov_events_count;
	uint16_t lifetime_cuv_events_count;
	uint16_t lifetime_ocd_events_count;
	uint16_t lifetime_occ_events_count;

	// Lifetime Block 7
	uint16_t lifetime_otf_events_count; // FET Over Temp
	uint16_t lifetime_valid_charge_term_count;
	uint16_t lifetime_fcc_update_count;
} BmsData;


typedef struct
{
	DriverType_e driver;
	union
	{
		AdcData adc;
		UartData uart;
		IoData io;
		IrqData irq;
		CanData can;
		GptData gpt;
		RtcData rtc;
		BmsData bms;
	} data;
} Message;