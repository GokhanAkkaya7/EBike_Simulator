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
	if (strcmp(driver_str, "rtc") == 0)
		return DRIVER_RTC;
	if (strcmp(driver_str, "bms") == 0)
		return DRIVER_BMS;
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

		case DRIVER_RTC:
		{
			cJSON* event_type = cJSON_GetObjectItem(data, "rtc_event");
			messages[i].data.rtc.rtc_event = (rtc_event_t)event_type->valueint;

			if (messages[i].data.rtc.rtc_event == RTC_SET)
			{
				cJSON* sec = cJSON_GetObjectItem(data, "second");
				cJSON* min = cJSON_GetObjectItem(data, "minute");
				cJSON* hour = cJSON_GetObjectItem(data, "hour");
				cJSON* day = cJSON_GetObjectItem(data, "day");
				cJSON* mon = cJSON_GetObjectItem(data, "month");
				cJSON* year = cJSON_GetObjectItem(data, "year");

				messages[i].data.rtc.second = sec->valueint;
				messages[i].data.rtc.minute = min->valueint;
				messages[i].data.rtc.hour = hour->valueint;
				messages[i].data.rtc.day = day->valueint;
				messages[i].data.rtc.month = mon->valueint;
				messages[i].data.rtc.year = year->valueint;
			}
			tx_event_flags_set(&timer_events, RTC_RECEIVE_EVENT, TX_OR);
			break;
		}
		case DRIVER_BMS:
		{
			cJSON* val; 
			BmsData* bms_data_ptr = &messages[i].data.bms;

			// ===============================================
			// Instant Values.
			// ===============================================
			val = cJSON_GetObjectItem(data, "current");
			if (val) bms_data_ptr->current = (int16_t)val->valueint;

			val = cJSON_GetObjectItem(data, "pack_voltage");
			if (val) bms_data_ptr->pack_voltage = (uint16_t)val->valueint;

			val = cJSON_GetObjectItem(data, "temperature");
			if (val) bms_data_ptr->temperature = (uint16_t)val->valueint;

			val = cJSON_GetObjectItem(data, "relative_soc");
			if (val) bms_data_ptr->relative_soc = (uint8_t)val->valueint;

			val = cJSON_GetObjectItem(data, "state_of_health");
			if (val) bms_data_ptr->state_of_health = (uint8_t)val->valueint;

			val = cJSON_GetObjectItem(data, "CycleCount");
			if (val) bms_data_ptr->cycle_count = (uint16_t)val->valueint;

			val = cJSON_GetObjectItem(data, "remaining_capacity");
			if (val) bms_data_ptr->remaining_capacity = (uint16_t)val->valueint;

			val = cJSON_GetObjectItem(data, "full_charge_capacity");
			if (val) bms_data_ptr->full_charge_capacity = (uint16_t)val->valueint;

			cJSON* cell_voltages_array = cJSON_GetObjectItem(data, "cell_voltages");
			if (cJSON_IsArray(cell_voltages_array)) {
				int count = cJSON_GetArraySize(cell_voltages_array);
				for (int j = 0; j < count && j < 15; ++j) {
					cJSON* cell = cJSON_GetArrayItem(cell_voltages_array, j);
					if (cell) bms_data_ptr->cell_voltages[j] = (uint16_t)cell->valueint;
				}
			}

			// ===============================================
			// State Flags.
			// ===============================================
			cJSON* status_array = cJSON_GetObjectItem(data, "safety_status");
			if (cJSON_IsArray(status_array)) {
				uint32_t temp_status = 0;
				for (int j = 0; j < cJSON_GetArraySize(status_array) && j < 4; ++j) {
					temp_status |= ((uint8_t)cJSON_GetArrayItem(status_array, j)->valueint << (j * 8));
				}
				bms_data_ptr->safety_status = temp_status;
			}

			status_array = cJSON_GetObjectItem(data, "charging_status");
			if (cJSON_IsArray(status_array)) {
				uint16_t temp_status = 0;
				for (int j = 0; j < cJSON_GetArraySize(status_array) && j < 2; ++j) {
					temp_status |= ((uint8_t)cJSON_GetArrayItem(status_array, j)->valueint << (j * 8));
				}
				bms_data_ptr->charging_status = temp_status;
			}

			status_array = cJSON_GetObjectItem(data, "operation_status");
			if (cJSON_IsArray(status_array)) {
				uint32_t temp_status = 0;
				for (int j = 0; j < cJSON_GetArraySize(status_array) && j < 4; ++j) {
					temp_status |= ((uint8_t)cJSON_GetArrayItem(status_array, j)->valueint << (j * 8));
				}
				bms_data_ptr->operation_status = temp_status;
			}

			// ===============================================
			// String and Info.
			// ===============================================
			val = cJSON_GetObjectItem(data, "manufacturer_name");
			if (cJSON_IsString(val)) strncpy(bms_data_ptr->manufacturer_name, val->valuestring, sizeof(bms_data_ptr->manufacturer_name) - 1);

			val = cJSON_GetObjectItem(data, "device_name");
			if (cJSON_IsString(val)) strncpy(bms_data_ptr->device_name, val->valuestring, sizeof(bms_data_ptr->device_name) - 1);


			//// ===============================================
			//// Configuration.
			//// ===============================================
			//cJSON* config_obj = cJSON_GetObjectItem(data, "Configuration");
			//if (cJSON_IsObject(config_obj))
			//{
			//	val = cJSON_GetObjectItem(config_obj, "CuvThreshold");
			//	if (val) bms_data_ptr->cuv_threshold = (uint16_t)val->valueint;
			//	val = cJSON_GetObjectItem(config_obj, "CuvRecovery");
			//	if (val) bms_data_ptr->cuv_recovery = (uint16_t)val->valueint;

			//	val = cJSON_GetObjectItem(config_obj, "CovThreshold");
			//	if (val) bms_data_ptr->cov_threshold = (uint16_t)val->valueint;
			//	val = cJSON_GetObjectItem(config_obj, "CovRecovery");
			//	if (val) bms_data_ptr->cov_recovery = (uint16_t)val->valueint;

			//	val = cJSON_GetObjectItem(config_obj, "OccThreshold");
			//	if (val) bms_data_ptr->occ_threshold = (int16_t)val->valueint;
			//	val = cJSON_GetObjectItem(config_obj, "OccRecovery");
			//	if (val) bms_data_ptr->occ_recovery = (int16_t)val->valueint;

			//	val = cJSON_GetObjectItem(config_obj, "OcdThreshold");
			//	if (val) bms_data_ptr->ocd_threshold = (int16_t)val->valueint;
			//	val = cJSON_GetObjectItem(config_obj, "OcdRecovery");
			//	if (val) bms_data_ptr->ocd_recovery = (int16_t)val->valueint;
			//}

			//// ===============================================
			//// Lifetime.
			//// ===============================================
			//cJSON* lifetime_obj = cJSON_GetObjectItem(data, "Lifetime");
			//if (cJSON_IsObject(lifetime_obj))
			//{
			//	cJSON* max_v_array = cJSON_GetObjectItem(lifetime_obj, "MaxCellVoltages");
			//	if (cJSON_IsArray(max_v_array)) {
			//		int count = cJSON_GetArraySize(max_v_array);
			//		for (int j = 0; j < count && j < 15; ++j) {
			//			bms_data_ptr->lifetime_max_cell_v[j] = (uint16_t)cJSON_GetArrayItem(max_v_array, j)->valueint;
			//		}
			//	}

			//	cJSON* min_v_array = cJSON_GetObjectItem(lifetime_obj, "MinCellVoltages");
			//	if (cJSON_IsArray(min_v_array)) {
			//		int count = cJSON_GetArraySize(min_v_array);
			//		for (int j = 0; j < count && j < 15; ++j) {
			//			bms_data_ptr->lifetime_min_cell_v[j] = (uint16_t)cJSON_GetArrayItem(min_v_array, j)->valueint;
			//		}
			//	}

			//	val = cJSON_GetObjectItem(lifetime_obj, "CovEventsCount");
			//	if (val) bms_data_ptr->lifetime_cov_events_count = (uint16_t)val->valueint;

			//	val = cJSON_GetObjectItem(lifetime_obj, "CuvEventsCount");
			//	if (val) bms_data_ptr->lifetime_cuv_events_count = (uint16_t)val->valueint;
			//}

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
	if (!root) return false; 

	for (int i = 0; i < count; ++i)
	{
		cJSON* item = cJSON_CreateObject();
		if (!item) {
			cJSON_Delete(root);
			return false;
		}

		cJSON* data = cJSON_CreateObject();
		if (!data) {
			cJSON_Delete(item);
			cJSON_Delete(root);
			return false;
		}

		switch (messages[i].driver)
		{

		case DRIVER_RTC:

			cJSON_AddStringToObject(item, "driver", "rtc");
			cJSON_AddStringToObject(item, "event", "RTC_SET"); 

			cJSON_AddNumberToObject(data, "year", messages[i].data.rtc.year);
			cJSON_AddNumberToObject(data, "month", messages[i].data.rtc.month);
			cJSON_AddNumberToObject(data, "day", messages[i].data.rtc.day);
			cJSON_AddNumberToObject(data, "hour", messages[i].data.rtc.hour);
			cJSON_AddNumberToObject(data, "minute", messages[i].data.rtc.minute);
			cJSON_AddNumberToObject(data, "second", messages[i].data.rtc.second);
			break;
		default:
			cJSON_Delete(data);
			cJSON_Delete(item);
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
	size_t json_len = strlen(json_str);
	if (json_len >= buffer_size)
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