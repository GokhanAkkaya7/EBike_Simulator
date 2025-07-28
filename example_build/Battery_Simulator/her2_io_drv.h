#pragma once
/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_io_drv.h
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : IO Driver module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 09.04.25
 ******************************************************************************************/

#ifndef SRC_HER2_IO_HER2_IO_DRV_H_
#define SRC_HER2_IO_HER2_IO_DRV_H_

 /*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(IO_DRV)
#include "conf_ioport_drv.h"
#include "driver_api.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef enum
{
    APP_IO_PORT_00_PIN_00 = 0,
    APP_IO_PORT_00_PIN_01,
    APP_IO_PORT_00_PIN_02,
    APP_IO_PORT_00_PIN_03,
    APP_IO_PORT_00_PIN_04,
    APP_IO_PORT_00_PIN_05,
    APP_IO_PORT_00_PIN_06,
    APP_IO_PORT_00_PIN_07,
    APP_IO_PORT_00_PIN_08,
    APP_IO_PORT_00_PIN_09,
    APP_IO_PORT_00_PIN_10,
    APP_IO_PORT_00_PIN_11,
    APP_IO_PORT_00_PIN_12,
    APP_IO_PORT_00_PIN_13,
    APP_IO_PORT_00_PIN_14,
    APP_IO_PORT_00_PIN_15,

    APP_IO_PORT_01_PIN_00,
    APP_IO_PORT_01_PIN_01,
    APP_IO_PORT_01_PIN_02,
    APP_IO_PORT_01_PIN_03,
    APP_IO_PORT_01_PIN_04,
    APP_IO_PORT_01_PIN_05,
    APP_IO_PORT_01_PIN_06,
    APP_IO_PORT_01_PIN_07,
    APP_IO_PORT_01_PIN_08,
    APP_IO_PORT_01_PIN_09,
    APP_IO_PORT_01_PIN_10,
    APP_IO_PORT_01_PIN_11,
    APP_IO_PORT_01_PIN_12,
    APP_IO_PORT_01_PIN_13,
    APP_IO_PORT_01_PIN_14,
    APP_IO_PORT_01_PIN_15,

    APP_IO_PORT_02_PIN_00,
    APP_IO_PORT_02_PIN_01,
    APP_IO_PORT_02_PIN_02,
    APP_IO_PORT_02_PIN_03,
    APP_IO_PORT_02_PIN_04,
    APP_IO_PORT_02_PIN_05,
    APP_IO_PORT_02_PIN_06,
    APP_IO_PORT_02_PIN_07,
    APP_IO_PORT_02_PIN_08,
    APP_IO_PORT_02_PIN_09,
    APP_IO_PORT_02_PIN_10,
    APP_IO_PORT_02_PIN_11,
    APP_IO_PORT_02_PIN_12,
    APP_IO_PORT_02_PIN_13,
    APP_IO_PORT_02_PIN_14,
    APP_IO_PORT_02_PIN_15,

    APP_IO_PORT_03_PIN_00,
    APP_IO_PORT_03_PIN_01,
    APP_IO_PORT_03_PIN_02,
    APP_IO_PORT_03_PIN_03,
    APP_IO_PORT_03_PIN_04,
    APP_IO_PORT_03_PIN_05,
    APP_IO_PORT_03_PIN_06,
    APP_IO_PORT_03_PIN_07,
    APP_IO_PORT_03_PIN_08,
    APP_IO_PORT_03_PIN_09,
    APP_IO_PORT_03_PIN_10,
    APP_IO_PORT_03_PIN_11,
    APP_IO_PORT_03_PIN_12,
    APP_IO_PORT_03_PIN_13,
    APP_IO_PORT_03_PIN_14,
    APP_IO_PORT_03_PIN_15,

    APP_IO_PORT_04_PIN_00,
    APP_IO_PORT_04_PIN_01,
    APP_IO_PORT_04_PIN_02,
    APP_IO_PORT_04_PIN_03,
    APP_IO_PORT_04_PIN_04,
    APP_IO_PORT_04_PIN_05,
    APP_IO_PORT_04_PIN_06,
    APP_IO_PORT_04_PIN_07,
    APP_IO_PORT_04_PIN_08,
    APP_IO_PORT_04_PIN_09,
    APP_IO_PORT_04_PIN_10,
    APP_IO_PORT_04_PIN_11,
    APP_IO_PORT_04_PIN_12,
    APP_IO_PORT_04_PIN_13,
    APP_IO_PORT_04_PIN_14,
    APP_IO_PORT_04_PIN_15,

    APP_IO_PORT_05_PIN_00,
    APP_IO_PORT_05_PIN_01,
    APP_IO_PORT_05_PIN_02,
    APP_IO_PORT_05_PIN_03,
    APP_IO_PORT_05_PIN_04,
    APP_IO_PORT_05_PIN_05,
    APP_IO_PORT_05_PIN_06,
    APP_IO_PORT_05_PIN_07,
    APP_IO_PORT_05_PIN_08,
    APP_IO_PORT_05_PIN_09,
    APP_IO_PORT_05_PIN_10,
    APP_IO_PORT_05_PIN_11,
    APP_IO_PORT_05_PIN_12,
    APP_IO_PORT_05_PIN_13,
    APP_IO_PORT_05_PIN_14,
    APP_IO_PORT_05_PIN_15,

    APP_IO_PORT_06_PIN_00,
    APP_IO_PORT_06_PIN_01,
    APP_IO_PORT_06_PIN_02,
    APP_IO_PORT_06_PIN_03,
    APP_IO_PORT_06_PIN_04,
    APP_IO_PORT_06_PIN_05,
    APP_IO_PORT_06_PIN_06,
    APP_IO_PORT_06_PIN_07,
    APP_IO_PORT_06_PIN_08,
    APP_IO_PORT_06_PIN_09,
    APP_IO_PORT_06_PIN_10,
    APP_IO_PORT_06_PIN_11,
    APP_IO_PORT_06_PIN_12,
    APP_IO_PORT_06_PIN_13,
    APP_IO_PORT_06_PIN_14,
    APP_IO_PORT_06_PIN_15,

    APP_IO_PORT_07_PIN_00,
    APP_IO_PORT_07_PIN_01,
    APP_IO_PORT_07_PIN_02,
    APP_IO_PORT_07_PIN_03,
    APP_IO_PORT_07_PIN_04,
    APP_IO_PORT_07_PIN_05,
    APP_IO_PORT_07_PIN_06,
    APP_IO_PORT_07_PIN_07,
    APP_IO_PORT_07_PIN_08,
    APP_IO_PORT_07_PIN_09,
    APP_IO_PORT_07_PIN_10,
    APP_IO_PORT_07_PIN_11,
    APP_IO_PORT_07_PIN_12,
    APP_IO_PORT_07_PIN_13,
    APP_IO_PORT_07_PIN_14,
    APP_IO_PORT_07_PIN_15,

    APP_IO_PORT_08_PIN_00,
    APP_IO_PORT_08_PIN_01,
    APP_IO_PORT_08_PIN_02,
    APP_IO_PORT_08_PIN_03,
    APP_IO_PORT_08_PIN_04,
    APP_IO_PORT_08_PIN_05,
    APP_IO_PORT_08_PIN_06,
    APP_IO_PORT_08_PIN_07,
    APP_IO_PORT_08_PIN_08,
    APP_IO_PORT_08_PIN_09,
    APP_IO_PORT_08_PIN_10,
    APP_IO_PORT_08_PIN_11,
    APP_IO_PORT_08_PIN_12,
    APP_IO_PORT_08_PIN_13,
    APP_IO_PORT_08_PIN_14,
    APP_IO_PORT_08_PIN_15,

    APP_IO_PORT_09_PIN_00,
    APP_IO_PORT_09_PIN_01,
    APP_IO_PORT_09_PIN_02,
    APP_IO_PORT_09_PIN_03,
    APP_IO_PORT_09_PIN_04,
    APP_IO_PORT_09_PIN_05,
    APP_IO_PORT_09_PIN_06,
    APP_IO_PORT_09_PIN_07,
    APP_IO_PORT_09_PIN_08,
    APP_IO_PORT_09_PIN_09,
    APP_IO_PORT_09_PIN_10,
    APP_IO_PORT_09_PIN_11,
    APP_IO_PORT_09_PIN_12,
    APP_IO_PORT_09_PIN_13,
    APP_IO_PORT_09_PIN_14,
    APP_IO_PORT_09_PIN_15,

    APP_IO_PORT_10_PIN_00,
    APP_IO_PORT_10_PIN_01,
    APP_IO_PORT_10_PIN_02,
    APP_IO_PORT_10_PIN_03,
    APP_IO_PORT_10_PIN_04,
    APP_IO_PORT_10_PIN_05,
    APP_IO_PORT_10_PIN_06,
    APP_IO_PORT_10_PIN_07,
    APP_IO_PORT_10_PIN_08,
    APP_IO_PORT_10_PIN_09,
    APP_IO_PORT_10_PIN_10,
    APP_IO_PORT_10_PIN_11,
    APP_IO_PORT_10_PIN_12,
    APP_IO_PORT_10_PIN_13,
    APP_IO_PORT_10_PIN_14,
    APP_IO_PORT_10_PIN_15,

    APP_IO_PORT_11_PIN_00,
    APP_IO_PORT_11_PIN_01,
    APP_IO_PORT_11_PIN_02,
    APP_IO_PORT_11_PIN_03,
    APP_IO_PORT_11_PIN_04,
    APP_IO_PORT_11_PIN_05,
    APP_IO_PORT_11_PIN_06,
    APP_IO_PORT_11_PIN_07,
    APP_IO_PORT_11_PIN_08,
    APP_IO_PORT_11_PIN_09,
    APP_IO_PORT_11_PIN_10,
    APP_IO_PORT_11_PIN_11,
    APP_IO_PORT_11_PIN_12,
    APP_IO_PORT_11_PIN_13,
    APP_IO_PORT_11_PIN_14,
    APP_IO_PORT_11_PIN_15,
} app_io_port_pin_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t io_init(void);
app_err_t set_pin_level(app_io_port_pin_t pin, app_io_level_t level);
app_err_t get_pin_level(app_io_port_pin_t pin, app_io_level_t* p_level);
app_err_t set_pin_perpsci1(app_io_port_pin_t pin, uint8_t channel_number);
app_err_t set_pin_ioout(app_io_port_pin_t pin, app_io_level_t level);
void IO_DATAHANDLER(IoData* p_incoming_data);

#endif
#endif
