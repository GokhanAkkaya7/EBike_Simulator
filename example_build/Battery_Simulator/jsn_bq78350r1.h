#pragma once
/******************************************************************************************
* © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
*
* File Name    : jsn_bq78350r1.h
* Author       : Gokhan Akkaya
* Description  : TI BQ78350-R1 gauge IC module.
* Project      : VESTEL Common modules project
* Creation Date: 19.04.2021
******************************************************************************************/

#ifndef JSN_GAUGE_JSN_BQ78350R1_H_
#define JSN_GAUGE_JSN_BQ78350R1_H_

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"
#if(BQ78350)

#include "conf_bq78350.h"
#include "Data_Handler.h"

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define MAX_BQ78350_RESP_SIZE                   50

#define BQ78350R1_OPERATSTATUS_BLOCK_SIZE       5
#define BQ78350R1_PROTECTION_TH_BLOCK_SIZE      35
#define BQ78350R1_CHARGINGSTATUS_BLOCK_SIZE     3
#define BQ78350R1_GAUGINGSTATUS_BLOCK_SIZE      3
#define BQ78350R1_DASTATUS2_BLOCK_SIZE          17
#define BQ78350R1_MANUFACSTATUS_BLOCK_SIZE      3
#define BQ78350R1_SAFETYSTATUS_BLOCK_SIZE       5
#define BQ78350R1_PFSTATUS_BLOCK_SIZE           5
#define BQ78350R1_CHEMID_BLOCK_SIZE             3
#define BQ78350R1_MANU_NAME_BLOCK_SIZE          30
#define BQ78350R1_MANU_INFO_BLOCK_SIZE          33
#define BQ78350R1_LIFETIME_BLOCK1_SIZE          33
#define BQ78350R1_LIFETIME_BLOCK2_SIZE          33
#define BQ78350R1_LIFETIME_BLOCK3_SIZE          15
#define BQ78350R1_LIFETIME_BLOCK4_SIZE          21
#define BQ78350R1_LIFETIME_BLOCK5_SIZE          19
#define BQ78350R1_LIFETIME_BLOCK6_SIZE          33
#define BQ78350R1_LIFETIME_BLOCK7_SIZE          33
#define BQ78350R1_SLEEP_WAKE_CMD_SIZE           3
#define BQ78350R1_BLOCK_W_ADRRESS_SIZE          2
#define BQ78350R1_CUV_TH_CMD_SIZE               4
#define GAUGE_IMPORT_BUFFER_LENGTH              10
#define BQ78350R1_DEVICE_NAME_BLOCK_SIZE        21
#define BQ78350R1_DEVICE_CHEM_BLOCK_SIZE        5
#define BQ78350R1_AUTHENTICATE_BLOCK_SIZE       21
#define BQ78350R1_SAFETYALERT_BLOCK_SIZE        5
#define BQ78350R1_PFALERT_BLOCK_SIZE            3
#define BQ78350R1_AFE_STATUS_BLOCK_SIZE         3
#define BQ78350R1_AFE_CONFIG_BLOCK_SIZE         11
#define BQ78350R1_AFE_VCX_BLOCK_SIZE            33
#define BQ78350R1_AFE_DATA_BLOCK_SIZE           14
#define BQ78350R1_DASTATUS1_BLOCK_SIZE          33
#define BQ78350R1_CUVSNAPSHOT_BLOCK_SIZE        33
#define BQ78350R1_COVSNAPSHOT_BLOCK_SIZE        33

/*----------------------------- Public Typedef Definitions -------------------------------*/

typedef struct
{
    uint16_t value;
    uint8_t valuep[MAX_BQ78350_RESP_SIZE];
    uint8_t p_size;
    uint8_t response;

} bq78350r1_resp_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

bool bq78350r1_init(void);
void bq78350_get_firmware_version(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_rtcurr(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_opstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_chgstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_gaugstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_packvolt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell15volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell14volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell13volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell12volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell11volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell10volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell9volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell8volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell7volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell6volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell5volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell4volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell3volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell2volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cell1volt(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_battemp(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_dastatus2(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_relsoc(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cyclecount(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_soh(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_manstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_safstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_pfstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_remcap(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_fullchgcap(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_da_conf(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_set_gpioctrl(uint16_t gpio_control_set);
bool bq78350r1_set_fetctrl(void);
bool bq78350r1_set_shutdown(void);
bool bq78350r1_set_gauge_reset(void);
bool bq78350r1_set_pf_clear(void);
bool bq78350r1_set_sleepmode(void);
bool bq78350r1_set_wakemode(void);
bool bq78350r1_set_manifacturer_no(uint8_t data_len, uint8_t* p_serial_no_str);
bool bq78350r1_set_curr_deadband_calib(uint8_t current_deadband);
bool bq78350r1_get_curr_deadband_calib(uint8_t* current_deadband);

#if(GAUGE_ADMIN_MODE)
bool bq78350_set_cuv_th(uint16_t* p_value);
bool bq78350_set_cuv_rec(uint16_t* p_value);
bool bq78350_set_cov_th(uint16_t* p_value);
bool bq78350_set_cov_rec(uint16_t* p_value);
bool bq78350_set_utd_th(int16_t* p_value);
bool bq78350_set_utd_rec(int16_t* p_value);
bool bq78350_set_utc_th(int16_t* p_value);
bool bq78350_set_utc_rec(int16_t* p_value);
bool bq78350_set_otd_th(int16_t* p_value);
bool bq78350_set_otd_rec(int16_t* p_value);
bool bq78350_set_otf_th(int16_t* p_value);
bool bq78350_set_otf_rec(int16_t* p_value);
bool bq78350_set_ocd_th(int16_t* p_value);
bool bq78350_set_ocd_rec(int16_t* p_value);
bool bq78350_set_occ_th(int16_t* p_value);
bool bq78350_set_occ_rec(int16_t* p_value);
bool bq78350_set_otc_th(int16_t* p_value);
bool bq78350_set_otc_rec(int16_t* p_value);
bool bq78350_import_gauge(uint8_t buff[GAUGE_IMPORT_BUFFER_LENGTH]);
#if(BQ78350_VERSION_R2)
bool bq78350_set_cov_low_th(uint16_t* p_value);
bool bq78350_set_cov_low_rec(uint16_t* p_value);
bool bq78350_set_cov_high_th(uint16_t* p_value);
bool bq78350_set_cov_high_rec(uint16_t* p_value);
bool bq78350_set_cov_recom_th(uint16_t* p_value);
bool bq78350_set_cov_recom_rec(uint16_t* p_value);
#endif
#endif

bool bq78350r1_get_abssoc(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_pendedv(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_avgtimeempty(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_avgtimefull(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_chemicalid(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_manufacturer_name(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_mandate(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_designcap(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_batstatus(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime1(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime2(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime3(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime4(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime5(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime6(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_lifetime7(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_maninfo(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_devname(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_otd_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_otd_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_oft_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_oft_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_doc_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_doc_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_uvt_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_uvt_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_ctl_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_ctl_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_dtl_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_dtl_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cov_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cov_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cuv_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_cuv_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_occ_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_occ_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_otc_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_otc_rec_th(bq78350r1_resp_t* p_gauge_resp);
bool bq78350r1_get_df_block(bq78350r1_resp_t* p_gauge_resp, uint8_t block_size, uint16_t block_reg);

#if(UNUSED_COMMANDS)
void get_dynamic_power(bq78350r1_resp_t* p_gauge_resp);
void get_ext_ave_cell_voltage(bq78350r1_resp_t* p_gauge_resp);
void get_safety_alert(bq78350r1_resp_t* p_gauge_resp);
void get_pf_alert(bq78350r1_resp_t* p_gauge_resp);
void get_afe_status(bq78350r1_resp_t* p_gauge_resp);
void get_afe_config(bq78350r1_resp_t* p_gauge_resp);
void get_afe_vcx(bq78350r1_resp_t* p_gauge_resp);
void get_afe_data(bq78350r1_resp_t* p_gauge_resp);
void get_dastatus1(bq78350r1_resp_t* p_gauge_resp);
void get_cuv_snapshot(bq78350r1_resp_t* p_gauge_resp);
void get_cov_snapshot(bq78350r1_resp_t* p_gauge_resp);
void get_hardware_version(bq78350r1_resp_t* p_gauge_resp);
void set_life_time_data_reset(bq78350r1_resp_t* p_gauge_resp);
void set_permanent_fail_data_reset(bq78350r1_resp_t* p_gauge_resp);
void set_device_reset(bq78350r1_resp_t* p_gauge_resp);
void get_remaining_capacity_alarm(bq78350r1_resp_t* p_gauge_resp);
void get_remaining_time_alarm(bq78350r1_resp_t* p_gauge_resp);
void get_battery_mode(bq78350r1_resp_t* p_gauge_resp);
void get_at_rate(bq78350r1_resp_t* p_gauge_resp);
void get_at_rate_time_to_full(bq78350r1_resp_t* p_gauge_resp);
void get_at_rate_time_to_empty(bq78350r1_resp_t* p_gauge_resp);
void get_at_rate_ok(bq78350r1_resp_t* p_gauge_resp);
void get_avarage_current(bq78350r1_resp_t* p_gauge_resp);
void get_max_error(bq78350r1_resp_t* p_gauge_resp);
void get_run_time_empty(bq78350r1_resp_t* p_gauge_resp);
void get_charging_current(bq78350r1_resp_t* p_gauge_resp);
void get_charging_voltage(bq78350r1_resp_t* p_gauge_resp);
void get_design_voltage(bq78350r1_resp_t* p_gauge_resp);
void get_specification_info(bq78350r1_resp_t* p_gauge_resp);
void get_device_chemistry(bq78350r1_resp_t* p_gauge_resp);
void get_gpio_status(bq78350r1_resp_t* p_gauge_resp);
void get_vaux_voltage(bq78350r1_resp_t* p_gauge_resp);
void get_authenticate(bq78350r1_resp_t* p_gauge_resp);
#endif

#endif
#endif
