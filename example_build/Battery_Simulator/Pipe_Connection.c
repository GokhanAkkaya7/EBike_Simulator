
#include "Pipe_Connection.h"
#include "config_simulator.h"
#include "Data_Handler.h"

HANDLE hPipe;

char pipe_buffer[DATA_BUFFER_SIZE];
Message upcoming_message[MAX_MESSAGE_NUMBER];

extern TX_SEMAPHORE data_ready_smph;
extern TX_SEMAPHORE buffer_free_smph;

void pipe_init(void)
{
	const wchar_t* pipeName = L"\\\\.\\pipe\\SimplePipe";
	hPipe = CreateNamedPipeW(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		512,                      // Out buffer size.
		512,                      // Entry buffer size.
		0,                        // Timeout.
		NULL
	);
}

void pipe_main(void)
{

	bool b_pipe_connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

	if (b_pipe_connected)
	{
		UINT status = tx_semaphore_get(&buffer_free_smph, TX_WAIT_FOREVER);

		if (TX_SUCCESS == status)
		{
			DWORD bytesRead;

			BOOL success_read = ReadFile(
				hPipe,
				pipe_buffer,
				sizeof(pipe_buffer) - 1,
				&bytesRead,
				NULL
			);

			if (success_read && bytesRead > 0)
			{
				pipe_buffer[bytesRead] = '\0'; // Null terminator
				tx_semaphore_put(&data_ready_smph);
			}
			else
			{
				CloseHandle(hPipe);
				tx_semaphore_put(&buffer_free_smph);
			}
		}


		//char write_buffer[20] = "Server Response";
		//DWORD bytesWritten;
		//BOOL success_write = WriteFile(hPipe, write_buffer, strlen(write_buffer), &bytesWritten, NULL);

		DisconnectNamedPipe(hPipe);

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