#pragma once

#include "conf_project_settings.h"
#include "config_simulator.h"
#include "Json.h"

void data_handler_init(void);
void update_main_data(Message* p_message, int msg_count);
void get_main_data(Message* p_get_data, DriverType_e desired_driver);