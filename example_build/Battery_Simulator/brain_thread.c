/* This is a small demo of the high-performance ThreadX kernel.  It includes examples of eight
   threads of different priorities, using a message queue, semaphore, mutex, event flags group,
   byte pool, and block pool.  */

#include	"Data_Distributor.h"
#include	"Data_Handler.h"
#include	"Pipe_Connection.h"
#include	"config_simulator.h"
#include	"her2_adc_fw.h"
#include	"her2_can_drv.h"
#include	"her2_irq_drv.h"
#include	"her2_gpt_drv.h"
#include	"tx_api.h"
#include	"stdio.h"
#include	"her2_gpt_drv_test.h"

#define     DEMO_STACK_SIZE				2048
#define     DEMO_BYTE_POOL_SIZE			32768
#define     DEMO_BLOCK_POOL_SIZE		100
#define     QUEUE_BUFFER_SIZE			256
#define		QUEUE_MESSAGE_SIZE			16

#define		THREAD_LOOP_TIMEOUT			100

   /* Define the ThreadX object control blocks...  */

TX_THREAD               thread_0;
TX_THREAD               thread_1;
TX_THREAD               thread_2;
TX_THREAD               thread_3;
TX_THREAD               thread_4;
TX_THREAD               thread_5;
TX_THREAD               thread_6;
TX_THREAD               thread_7;
TX_SEMAPHORE            data_ready_smph;
TX_SEMAPHORE            buffer_free_smph;
TX_MUTEX                pipe_mutex;
TX_EVENT_FLAGS_GROUP    event_flags_0;
TX_BYTE_POOL            byte_pool_0;
TX_BLOCK_POOL           block_pool_0;
TX_TIMER				timer_event;


/* Define thread prototypes.  */

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
void    thread_3_entry(ULONG thread_input);
void    thread_4_entry(ULONG thread_input);
void    thread_5_entry(ULONG thread_input);
void    thread_6_entry(ULONG thread_input);
void    thread_7_entry(ULONG thread_input);


/* Define main entry point.  */

int main()
{

	/* Enter the ThreadX kernel.  */
	tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void* first_unused_memory)
{

	CHAR* pointer = TX_NULL;

	/* Create a byte memory pool from which to allocate the thread stacks.  */
	tx_byte_pool_create(&byte_pool_0, "byte pool 0", first_unused_memory, DEMO_BYTE_POOL_SIZE);

	/* Put system definition stuff in here, e.g. thread creates and other assorted
	   create information.  */

	   /* Allocate the stack for thread 0.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	/* Create the main thread.  */
	tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
		pointer, DEMO_STACK_SIZE,
		1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);


	/* Allocate the stack for thread 1.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	/* Create threads 1 and 2. These threads pass information through a ThreadX
	   message queue.  It is also interesting to note that these threads have a time
	   slice.  */
	tx_thread_create(&thread_1, "thread 1", thread_1_entry, 1,
		pointer, DEMO_STACK_SIZE,
		16, 16, 4, TX_AUTO_START);

	/* Allocate the stack for thread 2.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	tx_thread_create(&thread_2, "thread 2", thread_2_entry, 2,
		pointer, DEMO_STACK_SIZE,
		16, 16, 4, TX_AUTO_START);

	/* Allocate the stack for thread 3.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	/* Create threads 3 and 4.  These threads compete for a ThreadX counting semaphore.
	   An interesting thing here is that both threads share the same instruction area.  */
	tx_thread_create(&thread_3, "thread 3", thread_3_entry, 3,
		pointer, DEMO_STACK_SIZE,
		8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Allocate the stack for thread 4.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	tx_thread_create(&thread_4, "thread 4", thread_4_entry, 4,
		pointer, DEMO_STACK_SIZE,
		8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Allocate the stack for thread 5.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	/* Create thread 5.  This thread simply pends on an event flag which will be set
	   by thread_0.  */
	tx_thread_create(&thread_5, "thread 5", thread_5_entry, 5,
		pointer, DEMO_STACK_SIZE,
		4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Allocate the stack for thread 6.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	/* Create threads 6 and 7.  These threads compete for a ThreadX mutex.  */
	tx_thread_create(&thread_6, "thread 6", thread_6_entry, 6,
		pointer, DEMO_STACK_SIZE,
		8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Allocate the stack for thread 7.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

	tx_thread_create(&thread_7, "thread 7", thread_7_entry, 7,
		pointer, DEMO_STACK_SIZE,
		8, 8, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Allocate the message queue.  */
	//tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, QUEUE_MESSAGE_SAIZE * sizeof(ULONG), TX_NO_WAIT);

	/* Create the message queue shared by threads 1 and 2.  */
	//tx_queue_create(&queue_0, "queue 0", QUEUE_MESSAGE_SIZE, pointer, QUEUE_BUFFER_SIZE * QUEUE_MESSAGE_SIZE);

	tx_semaphore_create(&data_ready_smph, "Data Ready Semaphore", 0);
	tx_semaphore_create(&buffer_free_smph, "Buffer Free Semaphore", 1);

	/* Create the event flags group used by threads 1 and 5.  */
	tx_event_flags_create(&event_flags_0, "event flags 0");

	tx_mutex_create(&pipe_mutex, "Pipe Mutex", TX_NO_INHERIT);

	/* Allocate the memory for a small block pool.  */
	tx_byte_allocate(&byte_pool_0, (VOID**)&pointer, DEMO_BLOCK_POOL_SIZE, TX_NO_WAIT);

	/* Create a block memory pool to allocate a message buffer from.  */
	tx_block_pool_create(&block_pool_0, "block pool 0", sizeof(ULONG), pointer, DEMO_BLOCK_POOL_SIZE);

	/* Allocate a block and release the block memory.  */
	tx_block_allocate(&block_pool_0, (VOID**)&pointer, TX_NO_WAIT);

	tx_timer_create(&timer_event, "Event Timer", timer_event_handler, 0, 1, 1, TX_AUTO_ACTIVATE);

	/* Release the block back to the pool.  */
	tx_block_release(pointer);

	pipe_init();

	data_handler_init();

	timer_event_handler_init();
}

/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{
	while (1)
	{
		UINT    status;
		timer_events_t tmr_events;

		test_app_init();

		while (1)
		{
			status = tx_event_flags_get(&timer_events, TIME_10MSEC_EVENT | TIME_20MSEC_EVENT | TIME_50MSEC_EVENT |
				TIME_100MSEC_EVENT | TIME_250MSEC_EVENT | TIME_500MSEC_EVENT | TIME_SEC_EVENT, TX_OR_CLEAR,
				&tmr_events.u32byte, THREAD_LOOP_TIMEOUT);

			if (tmr_events.bits.time_50msec_event)
			{
				Message main_adc_data = { 0 };
				get_main_data(&main_adc_data, DRIVER_ADC);

				ADC0_FW_CALLBACK(main_adc_data.data.adc.index, main_adc_data.data.adc.buffer);

			}

			if (tmr_events.bits.time_500msec_event)
			{
				Message main_can_data = { 0 };
				get_main_data(&main_can_data, DRIVER_CAN);
				CANBUS_DRV_CALLBACK(&main_can_data);
			}
			if (tmr_events.bits.time_100msec_event)
			{
				test_app_main();
			}
		}
	}
}


void    thread_1_entry(ULONG thread_input)
{
	UINT    status;
	timer_events_t tmr_events;

	while (1)

	{
		status = tx_event_flags_get(&timer_events, INTERRUPT_EVENT | GPT_RECEIVE_EVENT | IO_RECEIVE_EVENT, TX_OR_CLEAR,
			&tmr_events.u32byte, THREAD_LOOP_TIMEOUT);
		if (tmr_events.bits.interrupt_event)
		{
			Message main_irq_data = { 0 };
			get_main_data(&main_irq_data, DRIVER_IRQ);
			// TODO GA: Distribute callbacks in other way.
			IRQ_CALLBACK(&main_irq_data.data.irq);
		}
		if (tmr_events.bits.gpt_receive_event)
		{
			Message main_gpt_data = { 0 };
			get_main_data(&main_gpt_data, DRIVER_GPT);
			GPT_DATA_RECEIVE(&main_gpt_data.data.gpt);
			// TODO GA: Distribute callbacks in other way.
		}
		if (tmr_events.bits.io_receive_event)
		{
			Message main_io_data = { 0 };
			get_main_data(&main_io_data, DRIVER_IO);
			IO_DATAHANDLER(&main_io_data.data.io);
			// TODO GA: Distribute callbacks in other way.
		}
	}
}


void    thread_2_entry(ULONG thread_input)
{
	while (1)
	{
		pipe_main();
		tx_thread_sleep(10);
	}
}

void    thread_3_entry(ULONG thread_input)
{
	while (1)
	{
		pipe_data_receive();
		tx_thread_sleep(10);
	}
}

void    thread_4_entry(ULONG thread_input)
{
	UINT    status;

	while (1)
	{
		tx_thread_sleep(10);
	}
}

void    thread_5_entry(ULONG thread_input)
{
	UINT    status;

	while (1)
	{
		tx_thread_sleep(10);
	}
}


void    thread_6_entry(ULONG thread_input)
{
	UINT    status;

	while (1)
	{
		tx_thread_sleep(10);
	}
}

void    thread_7_entry(ULONG thread_input)
{
	UINT    status;

	while (1)
	{
		tx_thread_sleep(10);
	}
}
