#pragma once

#include "conf_project_settings.h"
#include "config_simulator.h"

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

typedef struct
{
	int channel_count;
	int Channel[12];
	int Period[12];
	GptUnit_e Unit[12];

} GptData;

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
	} data;
} Message;