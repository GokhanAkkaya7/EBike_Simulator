#pragma once

#include "config_simulator.h"
#include "driver_api.h"

void parser_json(const char* json_str, Message* messages, int* message_count);
bool processor_json(Message* messages, int count, char* buffer, size_t buffer_size);
