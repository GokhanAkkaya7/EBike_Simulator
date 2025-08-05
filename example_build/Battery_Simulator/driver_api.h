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
	} data;
} Message;