#pragma once

#include "conf_project_settings.h"
#include "config_simulator.h"

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
	int pin;
	int state; // 0 or 1
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
	uint16_t can_buffer[8]; // 0 or 1
} CanData;

typedef enum
{
	DRIVER_ADC = 0,
	DRIVER_UART,
	DRIVER_IO,
	DRIVER_IRQ,
	DRIVER_CAN,
	DRIVER_UNKNOWN
} DriverType_e;

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
	} data;
} Message;