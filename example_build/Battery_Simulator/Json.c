#include "Json.h"
#include "cJSON.h"
#include "Data_Distributor.h"

DriverType_e get_driver_type(const char* driver_str);

DriverType_e get_driver_type(const char* driver_str)
{
	if (strcmp(driver_str, "adc") == 0)
		return DRIVER_ADC;
	if (strcmp(driver_str, "uart") == 0)
		return DRIVER_UART;
	if (strcmp(driver_str, "io") == 0)
		return DRIVER_IO;
	if (strcmp(driver_str, "irq") == 0)
		return DRIVER_IRQ;
	if (strcmp(driver_str, "can") == 0)
		return DRIVER_CAN;
	if (strcmp(driver_str, "gpt") == 0)
		return DRIVER_GPT;
	return DRIVER_UNKNOWN;
}

void parser_json(const char* json_str, Message* messages, int* message_count)
{
	if (json_str == NULL || *json_str == '\0') {
		*message_count = 0;
		return;
	}

	cJSON* root = cJSON_Parse(json_str);

	if (root == NULL)
	{
		*message_count = 0;
		return;
	}

	if (!cJSON_IsArray(root))
	{
		cJSON_Delete(root);
		*message_count = 0;
		return;
	}

	int count = cJSON_GetArraySize(root);
	if (count > MAX_MESSAGE_NUMBER)
		count = MAX_MESSAGE_NUMBER;

	*message_count = count;

	for (int i = 0; i < count; ++i)
	{
		cJSON* item = cJSON_GetArrayItem(root, i);
		cJSON* driver = cJSON_GetObjectItem(item, "driver");
		cJSON* data = cJSON_GetObjectItem(item, "data");

		if (!driver || !cJSON_IsString(driver) || !data || !cJSON_IsObject(data)) {
			messages[i].driver = DRIVER_UNKNOWN;
			continue;
		}

		messages[i].driver = get_driver_type(driver->valuestring);

		switch (messages[i].driver)
		{
		case DRIVER_ADC:
		{
			cJSON* index = cJSON_GetObjectItem(data, "index");
			cJSON* buffer = cJSON_GetObjectItem(data, "buffer");
			messages[i].data.adc.index = index->valueint;
			messages[i].data.adc.buffer_len = cJSON_GetArraySize(buffer);
			for (int j = 0; j < messages[i].data.adc.buffer_len; ++j)
			{
				messages[i].data.adc.buffer[j] = cJSON_GetArrayItem(buffer, j)->valueint;
			}
			break;
		}
		case DRIVER_UART:
		{
			cJSON* value = cJSON_GetObjectItem(data, "value");
			strncpy(messages[i].data.uart.value, value->valuestring, sizeof(messages[i].data.uart.value));
			break;
		}
		case DRIVER_IO:
		{
			cJSON* pin = cJSON_GetObjectItem(data, "pin");
			cJSON* state = cJSON_GetObjectItem(data, "state");
			messages[i].data.io.pin = pin->valueint;
			messages[i].data.io.state = state->valueint;
			tx_event_flags_set(&timer_events, IO_RECEIVE_EVENT, TX_OR);
			break;
		}
		case DRIVER_IRQ:
		{
			cJSON* channel_count = cJSON_GetObjectItem(data, "channel_count");
			cJSON* channel_list = cJSON_GetObjectItem(data, "channel_list");
			messages[i].data.irq.channel_count = channel_count->valueint;
			for (int j = 0; j < messages[i].data.irq.channel_count; j++)
			{
				messages[i].data.irq.channel_list[j] = cJSON_GetArrayItem(channel_list, j)->valueint;
			}
			tx_event_flags_set(&timer_events, INTERRUPT_EVENT, TX_OR);
			break;
		}
		case DRIVER_CAN:
		{
			cJSON* id = cJSON_GetObjectItem(data, "id");
			cJSON* dlc = cJSON_GetObjectItem(data, "dlc");
			cJSON* buffer = cJSON_GetObjectItem(data, "can_buffer");
			messages[i].data.can.id = id->valueint;
			messages[i].data.can.dlc = dlc->valueint;
			for (int j = 0; j < 8; j++)
			{
				messages[i].data.can.can_buffer[j] = cJSON_GetArrayItem(buffer, j)->valueint;
			}
			break;
		}
		case DRIVER_GPT:
		{
			cJSON* channel_count = cJSON_GetObjectItem(data, "channel_count");
			cJSON* Channel = cJSON_GetObjectItem(data, "Channel");
			cJSON* Period = cJSON_GetObjectItem(data, "Period");
			cJSON* Unit = cJSON_GetObjectItem(data, "Unit");
			messages[i].data.gpt.channel_count = channel_count->valueint;
			for (int j = 0; j < channel_count->valueint; j++)
			{
				messages[i].data.gpt.Channel[j] = cJSON_GetArrayItem(Channel, j)->valueint;
				messages[i].data.gpt.Period[j] = cJSON_GetArrayItem(Period, j)->valueint;
				messages[i].data.gpt.Unit[j] = cJSON_GetArrayItem(Unit, j)->valueint;
			}
			tx_event_flags_set(&timer_events, GPT_RECEIVE_EVENT, TX_OR);
			break;
		}
		default:
			break;
		}
	}

	cJSON_Delete(root);
}

bool processor_json(Message* messages, int count, char* buffer, size_t buffer_size)
{
	if (!buffer || buffer_size == 0)
		return false;

	cJSON* root = cJSON_CreateArray();

	for (int i = 0; i < count; ++i)
	{
		cJSON* item = cJSON_CreateObject();
		cJSON* data = cJSON_CreateObject();

		switch (messages[i].driver)
		{
		case DRIVER_ADC:
			cJSON_AddStringToObject(item, "driver", "adc");
			cJSON_AddNumberToObject(data, "index", messages[i].data.adc.index);
			cJSON* arr = cJSON_CreateIntArray((const int*)messages[i].data.adc.buffer, messages[i].data.adc.buffer_len);
			cJSON_AddItemToObject(data, "buffer", arr);
			break;

		case DRIVER_UART:
			cJSON_AddStringToObject(item, "driver", "uart");
			cJSON_AddStringToObject(data, "value", messages[i].data.uart.value);
			break;

		case DRIVER_IO:
			cJSON_AddStringToObject(item, "driver", "io");
			cJSON_AddNumberToObject(data, "pin", messages[i].data.io.pin);
			cJSON_AddBoolToObject(data, "state", messages[i].data.io.state);
			break;

		case DRIVER_IRQ:
			cJSON_AddStringToObject(item, "driver", "irq");
			cJSON_AddNumberToObject(data, "id", messages[i].data.irq.channel_count);
			cJSON_AddBoolToObject(data, "triggered", messages[i].data.irq.channel_list);
			break;
		case DRIVER_CAN:
			cJSON_AddStringToObject(item, "driver", "irq");
			cJSON_AddNumberToObject(data, "id", messages[i].data.can.id);
			cJSON_AddBoolToObject(data, "triggered", messages[i].data.can.dlc);
			cJSON_AddBoolToObject(data, "triggered", messages[i].data.can.can_buffer);
			break;

		default:
			continue;
		}

		cJSON_AddItemToObject(item, "data", data);
		cJSON_AddItemToArray(root, item);
	}

	char* json_str = cJSON_PrintUnformatted(root);
	if (!json_str)
	{
		cJSON_Delete(root);
		return false;
	}

	if (strlen(json_str) >= buffer_size)
	{
		free(json_str);
		cJSON_Delete(root);
		return false;
	}

	strncpy(buffer, json_str, buffer_size - 1);
	buffer[buffer_size - 1] = '\0';

	free(json_str);
	cJSON_Delete(root);

	return true;
}
