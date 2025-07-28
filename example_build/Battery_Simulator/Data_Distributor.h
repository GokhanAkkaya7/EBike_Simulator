#pragma once

#include "conf_project_settings.h"

#define TIME_10MSEC_EVENT                   0x01
#define TIME_20MSEC_EVENT					0x02
#define TIME_50MSEC_EVENT                   0x04
#define TIME_100MSEC_EVENT                  0x08
#define TIME_250MSEC_EVENT                  0x10
#define TIME_500MSEC_EVENT                  0x20
#define TIME_SEC_EVENT                      0x40
#define INTERRUPT_EVENT						0x80
#define GPT_RECEIVE_EVENT					0x100
#define IO_RECEIVE_EVENT					0x200

typedef union
{
	struct
	{
		uint32_t time_10msec_event	: 1;
		uint32_t time_20msec_event	: 1;
		uint32_t time_50msec_event	: 1;
		uint32_t time_100msec_event : 1;
		uint32_t time_250msec_event : 1;
		uint32_t time_500msec_event : 1;
		uint32_t time_sec_event		: 1;
		uint32_t interrupt_event	: 1;
		uint32_t gpt_receive_event	: 1;
		uint32_t io_receive_event	: 1;
	}bits;
	uint32_t u32byte;
}timer_events_t;

TX_EVENT_FLAGS_GROUP timer_events;

void timer_event_handler_init(void);
void timer_event_handler(void);