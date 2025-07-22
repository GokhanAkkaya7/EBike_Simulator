#include "Data_Handler.h"
#include "Json.h"

TX_MUTEX data_mutex;
Message Message_storage[MAX_MESSAGE_NUMBER];
int total_message_number = 0;

void data_handler_init(void)
{
    tx_mutex_create(&data_mutex, "data_mutex is created", TX_NO_INHERIT);
}
void update_main_data(Message* p_message, int msg_count)
{
    tx_mutex_get(&data_mutex, TX_WAIT_FOREVER);

    total_message_number = msg_count;

    if (msg_count > MAX_MESSAGE_NUMBER)
        msg_count = MAX_MESSAGE_NUMBER;

    memcpy(Message_storage, p_message, sizeof(Message) * msg_count);

    tx_mutex_put(&data_mutex);
}

void get_main_data(Message* p_get_data, DriverType_e desired_driver)
{
    tx_mutex_get(&data_mutex, TX_WAIT_FOREVER);

    for (int data_index = 0; data_index < total_message_number; data_index++)
        if (desired_driver == Message_storage[data_index].driver)       
            *p_get_data = Message_storage[data_index];          
        
    tx_mutex_put(&data_mutex);
}
