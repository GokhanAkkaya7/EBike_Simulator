/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_flash_drv.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : Flash Driver HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 14.04.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_flash_drv.h"

#if(FLASH_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define SIM_DATA_FLASH_FILENAME				"dataflash.bin"
#define SIM_DATA_FLASH_SIZE					(DATAFLASH_BLOCK_TOTAL_NUMBER * DATAFLASH_BLOCK_SIZE)
#define CHUNK_SIZE							(DATAFLASH_BLOCK_SIZE) // Define a 1KB chunk size for writing.
#define FLASH_ERASED_VALUE					(0xFF)

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX flash_rdy_lock;
static bool g_is_initialized = false;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static app_err_t check_and_create_flash_file(void);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: check_and_create_flash_file
 *
 * Description  : Checks if the simulation file ('dataflash.bin') exists in the
 *                working directory. If not, it creates and initializes it by writing
 *                small chunks to avoid large memory allocation (malloc).
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS  - If the file exists or was created successfully.
 *                APP_FAIL	   - Otherwise.
 *****************************************************************************************/
static app_err_t check_and_create_flash_file(void)
{
	FILE* fp = fopen(SIM_DATA_FLASH_FILENAME, "rb");			// Try to open the file for reading to check if it exists.

	if (fp == NULL)
	{
		fp = fopen(SIM_DATA_FLASH_FILENAME, "wb");				// File does not exist, so create it and fill it with 0xFF.
		if (fp == NULL)
			return APP_FAIL;


		uint8_t chunk_buffer[CHUNK_SIZE];
		memset(chunk_buffer, FLASH_ERASED_VALUE, CHUNK_SIZE);	// Fill the buffer with the erased state value once.

		size_t bytes_remaining = SIM_DATA_FLASH_SIZE;
		while (bytes_remaining > 0)
		{
			size_t bytes_to_write = (bytes_remaining < CHUNK_SIZE) ? bytes_remaining : CHUNK_SIZE;
			size_t written = fwrite(chunk_buffer, 1, bytes_to_write, fp);

			if (written != bytes_to_write)
			{
				fclose(fp);
				return APP_FAIL;
			}
			bytes_remaining -= written;
		}
		fclose(fp);
	}
	else
		fclose(fp);												// File already exists, nothing to do.


	return APP_SUCCESS;
}

/*----------------------------------- Public Functions -----------------------------------*/


/******************************************************************************************
 * Function Name: flash_drv_configure
 *
 * Description  : Configure Flash Driver HAL. Call this method only once.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *******************************************************************************************/
app_err_t flash_drv_configure(void)
{
	if (g_is_initialized)
		return APP_SUCCESS;

	// Create a mutex for thread-safe file access.
	UINT status = tx_mutex_create(&flash_rdy_lock, (CHAR*)"Flash Sim Lock", TX_INHERIT);
	if (TX_SUCCESS != status)
		return APP_ERR_THREADX;


	// Check for the flash file and create if it doesn't exist.
	if (APP_SUCCESS != check_and_create_flash_file())
	{
		tx_mutex_delete(&flash_rdy_lock);
		return APP_FAIL;
	}

	g_is_initialized = true;
	return APP_SUCCESS;
}

/******************************************************************************************
 * Function Name: flash_drv_init
 *
 * Description  : This function calls HAL API to initialize FLASH LP HAL driver.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *****************************************************************************************/
app_err_t flash_drv_init(void)
{
	UINT tx_res = tx_mutex_get(&flash_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_is_initialized)
		result = APP_ERR_DRV_CONFIGURE;

	tx_res = tx_mutex_put(&flash_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/*********************************************************** Data Flash Methods *******************************************************/

/******************************************************************************************
 * Function Name: dflash_drv_block_write
 *
 * Description  : This function first erases chosen the block which the data will be written,
 *                and then writes the data to the chosen data flash block
 *
 * Arguments    : uint8_t *p_write_buffer       - pointer to write_array
 *                uint32_t block_address        - flash block which the data will be written
 *                uint32_t num_bytes            - number of bytes that will be written
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *                APP_FAIL                      Application failed.
 *****************************************************************************************/
app_err_t dflash_drv_block_write(uint8_t const* const p_write_buffer, uint32_t block_address, uint32_t num_bytes)
{
	UINT tx_res = tx_mutex_get(&flash_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_initialized)
		result = APP_ERR_DRV_CONFIGURE;
	else if (p_write_buffer == NULL)
		result = APP_ERR_ASSERTION;
	else if ((block_address + num_bytes) > SIM_DATA_FLASH_SIZE)
		result = APP_ERR_ASSERTION;
	else
	{
		FILE* fp = fopen(SIM_DATA_FLASH_FILENAME, "r+b"); // Open for read/write in binary mode

		if (fp != NULL)
		{
			if (fseek(fp, block_address, SEEK_SET) == 0)
			{
				size_t bytes_written = fwrite(p_write_buffer, 1, num_bytes, fp);
				if (bytes_written == num_bytes)
				{
					result = APP_SUCCESS;
					printf("[SIM-FLASH] Wrote %u bytes to address 0x%X.\n", (unsigned int)num_bytes, (unsigned int)block_address);
				}
				else
					printf("[SIM-FLASH] ERROR: Failed to write all bytes to file.\n");
			}
			else
				printf("[SIM-FLASH] ERROR: Failed to seek to address 0x%X in file.\n", (unsigned int)block_address);
			fclose(fp);
		}
		else
			printf("[SIM-FLASH] ERROR: Could not open virtual flash file for writing.\n");
	}

	tx_res = tx_mutex_put(&flash_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: dflash_drv_block_read
 *
 * Description  : This function first reads the data from the chosen the block.
 *
 * Arguments    : uint8_t *p_read_buffer    - pointer to read_array
 *                uint32_t block_address    - flash block which the data will be read
 *                uint32_t num_bytes        - number of bytes that will be read
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 *****************************************************************************************/
app_err_t dflash_drv_block_read(uint8_t* p_read_buffer, uint32_t block_address, uint32_t num_bytes)
{
	UINT tx_res = tx_mutex_get(&flash_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_initialized)
		result = APP_ERR_DRV_CONFIGURE;

	else if (p_read_buffer == NULL)
		result = APP_ERR_ASSERTION;
	else if ((block_address + num_bytes) > SIM_DATA_FLASH_SIZE)
		result = APP_ERR_ASSERTION;
	else
	{
		FILE* fp = fopen(SIM_DATA_FLASH_FILENAME, "rb"); // Open for reading in binary mode

		if (fp != NULL)
		{
			if (fseek(fp, block_address, SEEK_SET) == 0)
			{
				size_t bytes_read = fread(p_read_buffer, 1, num_bytes, fp);
				if (bytes_read == num_bytes)
				{	
					result = APP_SUCCESS;
					printf("[SIM-FLASH] Read operation is succeded.\n");
				}				
				else
					printf("[SIM-FLASH] ERROR: Failed to read all requested bytes from file.\n");
			}
			else
				printf("[SIM-FLASH] ERROR: Failed to seek to address 0x%X in file.\n", (unsigned int)block_address);
			fclose(fp);
		}
		else
			printf("[SIM-FLASH] ERROR: Could not open virtual flash file for reading.\n");
	}

	tx_res = tx_mutex_put(&flash_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/*************************************************** Code Flash Methods ****************************************************************/

/******************************************************************************************
 * Function Name: cflash_drv_block_erase
 *
 * Description  : This function erases the chosen code flash block as long as it is in
 *                code flash range.
 *                CAUTION: This driver is blocking due to code flash HW.
 *
 * Arguments    : uint32_t block address
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *****************************************************************************************/
app_err_t cflash_drv_block_erase(uint32_t block_address)
{
	(void)block_address;
	app_err_t result = APP_SUCCESS;
	return result;
}

/******************************************************************************************
 * Function Name: cflash_drv_block_write
 *
 * Description  : This function writes the given amount of data adjusted to multiple of min page write
 *                to the chosen code flash block as long as address is in code flash range.
 *                CAUTION:  This write function will automatically overwrite the following bytes to
 *                complete a write page. It is also blocking due to code flash HW
 *
 * Arguments    : uint8_t *p_write_buffer   - pointer to write_array
 *                uint32_t block_address    - flash block which the data will be written
 *                uint32_t num_bytes        - number of bytes that will be written
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *****************************************************************************************/
app_err_t cflash_drv_block_write(uint8_t const* const p_write_buffer, uint32_t block_address, uint32_t num_bytes)
{
	(void)p_write_buffer; (void)block_address; (void)num_bytes;
	app_err_t result = APP_SUCCESS;
	return result;
}

/******************************************************************************************
 * Function Name: cflash_drv_block_read
 *
 * Description  : Reads given amount of bytes of data from the chosen the block
 *                CAUTION: This driver is blocking due to code flash HW.
 *
 * Arguments    : uint8_t *p_read_buffer    - pointer to read_array
 *                uint32_t block_address    - flash block which the data will be read
 *                uint32_t num_bytes        - number of bytes that will be read
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *****************************************************************************************/
app_err_t cflash_drv_block_read(uint8_t* p_read_buffer, uint32_t block_address, uint32_t num_bytes)
{
	(void)p_read_buffer; (void)block_address; (void)num_bytes;
	app_err_t result = APP_SUCCESS;
	return result;
}

/******************************************************************************************
 * Function Name: cflash_drv_block_blank_check
 *
 * Description  : This function checks if given block is blank or not. One must give start
 * address of the block, blank result returns in p_is_blank parameter.
 *
 * Arguments    : uint32_t block_address
 *                bool *p_is_blank
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_ASSERTION             Given parameter is not appropriate.
 *****************************************************************************************/
app_err_t cflash_drv_block_blank_check(uint32_t block_address, bool* p_is_blank)
{
	(void)block_address;
	app_err_t result = APP_SUCCESS;
	return result;
}

/******************************************************************************************
 * Function Name: FLASH_DRV_CALLBACK
 *
 * Description  : Flash Driver Callback Function.
 *
 * Arguments    : None
 *
 * Return Value : None
 *****************************************************************************************/
void FLASH_DRV_CALLBACK(void)
{

}

#endif
