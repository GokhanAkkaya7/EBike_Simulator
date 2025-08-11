
#include "Data_Distributor.h"
#include "Data_Handler.h"
#include "config_simulator.h"

#define MSEC_10_CATCHER						1
#define MSEC_20_CATCHER						2
#define MSEC_50_CATCHER						5
#define MSEC_100_CATCHER					10
#define MSEC_250_CATCHER					25
#define MSEC_500_CATCHER					50
#define SEC_1_CATCHER						100


void timer_event_handler_init(void)
{
	UINT err_timer_events = tx_event_flags_create(&timer_events, (CHAR*)"Timer Event Flags");
}

void timer_event_handler(ULONG timer_input)
{
	(void)timer_input;
	static uint8_t timer_tick_count = 0; 

	timer_tick_count++; // Every tick means 10ms

	if (0 == (timer_tick_count % MSEC_10_CATCHER))
		tx_event_flags_set(&timer_events, TIME_10MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % MSEC_20_CATCHER))
		tx_event_flags_set(&timer_events, TIME_20MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % MSEC_50_CATCHER))
		tx_event_flags_set(&timer_events, TIME_50MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % MSEC_100_CATCHER))
		tx_event_flags_set(&timer_events, TIME_100MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % MSEC_250_CATCHER))
		tx_event_flags_set(&timer_events, TIME_250MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % MSEC_500_CATCHER))
		tx_event_flags_set(&timer_events, TIME_500MSEC_EVENT, TX_OR);

	if (0 == (timer_tick_count % SEC_1_CATCHER))
	{
		tx_event_flags_set(&timer_events, TIME_SEC_EVENT, TX_OR);
		timer_tick_count = 0;
	}
}