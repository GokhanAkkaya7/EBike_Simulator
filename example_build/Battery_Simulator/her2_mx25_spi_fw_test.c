/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_spi_mx25_fw_test.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Test application of SPI driver.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 14.03.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_mx25_spi_fw_test.h"

#if(SPI_TEST)
#include "her2_mx25_spi_fw.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define FW_TEST_DATA_SIZE                   320

/* Sample address for testing External Flash. Do not give 0x0000 to avoid stucking in assertion error.*/
#define FLASH_OP_START_ADDRESS              0x0001 // TODO : Do not let giving addrress of 0X0000 on hal side.

/*------------------------------------ Static Variables ----------------------------------*/

/* Test procedure control flags*/
static volatile bool spi_fw_read_test_res = false;
static volatile bool spi_fw_erase_test_res = false;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: spi_fw_app_init
 *
 * Description  : Initialize application layer of the SPI FW external flash module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
bool spi_test_init(void)
{
    if (APP_SUCCESS == mx25_configure())
        if (APP_SUCCESS == mx25_open())
            return true;

    return false;
}

/******************************************************************************************
 * Function Name: spi_fw_app_test
 *
 * Description  : Test method of spi fw external flash module. Do basic operations to test
 * if spi driver and mx25 are working as expected.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void spi_test_main(void)
{
    spi_fw_read_test_res = false;
    spi_fw_erase_test_res = false;

    uint16_t success_cnt = 0;
    uint8_t t_data[FW_TEST_DATA_SIZE];          // Data to be written in external flash.
    uint8_t c_data[FW_TEST_DATA_SIZE];          // Buffer to read from related external flash area.

    for (uint16_t indy = 0; indy < FW_TEST_DATA_SIZE; indy++)
        t_data[indy] = (uint8_t)indy;

    /*Check if the write operation realized successfully (but first erase the related area) and then check if the
     * erase operation is done correctly (the values in the external memory must be 0xFF after the erase operation.)*/

    if (APP_SUCCESS == mx25_sector_erase(FLASH_OP_START_ADDRESS))
        if (APP_SUCCESS == mx25_write(FLASH_OP_START_ADDRESS, t_data, FW_TEST_DATA_SIZE))
            if (APP_SUCCESS == mx25_read(FLASH_OP_START_ADDRESS, c_data, FW_TEST_DATA_SIZE))
                if (0 == strcmp((char*)t_data, (char*)c_data))
                    spi_fw_read_test_res = true;

    if (APP_SUCCESS == mx25_sector_erase(FLASH_OP_START_ADDRESS))
        if (APP_SUCCESS == mx25_read(FLASH_OP_START_ADDRESS, c_data, FW_TEST_DATA_SIZE))
            for (uint16_t indx = 0; indx < FW_TEST_DATA_SIZE; indx++)
            {
                if (0xFF == c_data[indx])
                    success_cnt++;
            }
    if (success_cnt == FW_TEST_DATA_SIZE)
        spi_fw_erase_test_res = true;
}

#endif
