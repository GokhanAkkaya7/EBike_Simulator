
#include "Pipe_Connection.h"
#include "config_simulator.h"
#include "Data_Handler.h"

HANDLE hPipe;

char pipe_buffer[DATA_BUFFER_SIZE];
Message upcoming_message[MAX_MESSAGE_NUMBER];
bool g_is_client_connected = false;

extern TX_SEMAPHORE data_ready_smph;
extern TX_SEMAPHORE buffer_free_smph;
extern TX_SEMAPHORE g_outgoing_message_semaphore;
extern TX_QUEUE g_outgoing_message_queue;

static void pipe_read_data(void);
static void pipe_write_data(void);

void pipe_init(void)
{
	// Pipe'ý SADECE BÝR KEZ burada oluþturuyoruz.
	const wchar_t* pipeName = L"\\\\.\\pipe\\SimplePipe";
	hPipe = CreateNamedPipeW(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1, DATA_BUFFER_SIZE, DATA_BUFFER_SIZE, 0, NULL
	);
}

void pipe_main_loop(void)
{
	while (1)
	{
		printf("[PIPE] Waiting for client to connect...\n");
		g_is_client_connected = false;

		if (ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED))
		{
			printf("[PIPE] Client connected. Entering I/O loop.\n");
			g_is_client_connected = true;

			while (g_is_client_connected)
			{
				pipe_read_data();

				pipe_write_data();

				tx_thread_sleep(10);
			}
			printf("[PIPE] Client disconnected. Ready for new connection.\n");
			DisconnectNamedPipe(hPipe);
		}
		else
		{
			tx_thread_sleep(100);
		}
	}
}

static void pipe_read_data(void)
{
	UINT status = tx_semaphore_get(&buffer_free_smph, TX_WAIT_FOREVER);
	if (status == TX_SUCCESS)
	{
		DWORD bytesRead;
		BOOL success_read = ReadFile(hPipe, pipe_buffer, sizeof(pipe_buffer) - 1, &bytesRead, NULL);

		if (success_read && bytesRead > 0)
		{
			pipe_buffer[bytesRead] = '\0';
			tx_semaphore_put(&data_ready_smph);
		}
		else
		{
			tx_semaphore_put(&buffer_free_smph);
			g_is_client_connected = false;
		}
	}
}

static void pipe_write_data(void)
{
	if (tx_semaphore_get(&g_outgoing_message_semaphore, TX_NO_WAIT) == TX_SUCCESS)
	{
		Message* message_to_send = NULL;
		if (tx_queue_receive(&g_outgoing_message_queue, &message_to_send, TX_NO_WAIT) == TX_SUCCESS)
		{
			char json_buffer[DATA_BUFFER_SIZE];
			if (processor_json(message_to_send, 1, json_buffer, sizeof(json_buffer)))
			{
				DWORD bytesWritten;
				char final_buffer[DATA_BUFFER_SIZE];
				snprintf(final_buffer, sizeof(final_buffer), "%s\n", json_buffer);

				if (!WriteFile(hPipe, final_buffer, strlen(final_buffer), &bytesWritten, NULL))
				{
					g_is_client_connected = false;
				}
			}
		}
	}
}

void pipe_data_receive(void)
{

	UINT status = tx_semaphore_get(&data_ready_smph, TX_WAIT_FOREVER);

	if (status == TX_SUCCESS)
	{
		int msg_count = 0;

		parser_json(pipe_buffer, upcoming_message, &msg_count);
		update_main_data(upcoming_message, msg_count);
	}

	tx_semaphore_put(&buffer_free_smph);

}