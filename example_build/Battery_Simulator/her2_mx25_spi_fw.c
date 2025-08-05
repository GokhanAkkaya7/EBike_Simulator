/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_mx25_spi_fw.c
 * Author       : Gokhan Akkaya
 * Device(s)    : Renesas Synergy Family
 * Tool-Chain   : GCC ARM Embedded 10.3.1.20210824
 * Description  : SPI (MX25 external module) HAL module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer
 * Creation Date: 14.03.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_mx25_spi_fw.h"

#if(MX25_SPI_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define SIM_EXT_FLASH_FILENAME              "external_flash.bin"
#define FLASH_ERASED_VALUE                  (0xFF)
#define ERASE_CHUNK_SIZE                    (256)				// Macronix Page Size.

#define MACRONIX_SECTOR_SIZE				(0x001000)			// 4kB
#define MACRONIX_CHIP_SIZE                  (0x400000)			// 4MB

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX mx25_rdy_lock;
static bool g_is_configured = false;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static bool check_and_create_ext_flash_file(void);
static bool internal_file_erase(uint32_t address, uint32_t size);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: check_and_create_ext_flash_file.
 *
 * Description  : Checks if the simulation file exists. If not, creates it.
 *
 * Arguments    : None
 *
 * Return Value : Bool
 *****************************************************************************************/
static bool check_and_create_ext_flash_file(void)
{
	FILE* fp = NULL;
	errno_t err;

	err = fopen_s(&fp, SIM_EXT_FLASH_FILENAME, "rb");
	if (err != 0)
	{
		err = fopen_s(&fp, SIM_EXT_FLASH_FILENAME, "wb");
		if ((err != 0) && (NULL == fp))
			return false;

		// Just create the file. The actual erase (filling with 0xFF) will happen
		// when an erase function is called for the first time.
		fclose(fp);
	}
	else
		fclose(fp);

	return true;
}

/******************************************************************************************
 * Function Name: internal_file_erase
 *
 * Description  : Helper function to fill a region of the file with 0xFF.
 *
 * Arguments    : uint32_t address, uint32_t size
 *
 * Return Value : Bool
 *****************************************************************************************/
static bool internal_file_erase(uint32_t address, uint32_t size)
{
	if ((address + size) > MACRONIX_CHIP_SIZE)
	{
		printf("[SIM-MX25] ERROR: Erase address/size out of bounds (Addr: 0x%X, Size: %u).\n", (unsigned int)address, (unsigned int)size);
		return false;
	}

	bool result = false;

	FILE* fp = fopen(SIM_EXT_FLASH_FILENAME, "r+b");    // Open for read/write
	if (fp != NULL)
	{
		if (fseek(fp, address, SEEK_SET) == 0)
		{
			uint8_t erase_buffer[ERASE_CHUNK_SIZE];
			memset(erase_buffer, FLASH_ERASED_VALUE, ERASE_CHUNK_SIZE);

			uint32_t remaining = size;
			while (remaining > 0)
			{
				uint32_t to_write = (remaining < ERASE_CHUNK_SIZE) ? remaining : ERASE_CHUNK_SIZE;
				if (fwrite(erase_buffer, 1, to_write, fp) != to_write)
				{
					printf("[SIM-MX25] ERROR: Failed while writing erase chunk to file.\n");
					goto cleanup;                       // Exit loop and cleanup
				}
				remaining -= to_write;
			}
			result = true;
		}
	cleanup:
		fclose(fp);
	}
	else
		printf("[SIM-MX25] ERROR: Could not open file for erasing.\n");

	return result;
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: mx25_configure
 *
 * Description  : Configure MX25 Framework HAL. Call this method only once.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *******************************************************************************************/
app_err_t mx25_configure(void)
{
	if (g_is_configured)
		return APP_SUCCESS;

	UINT err_mx25_rdy_lock = tx_mutex_create(&mx25_rdy_lock, (CHAR*)"MX25 Ready Lock", TX_INHERIT);
	if (TX_SUCCESS != err_mx25_rdy_lock)
		return APP_ERR_THREADX;

	if (!check_and_create_ext_flash_file())
	{
		tx_mutex_delete(&mx25_rdy_lock);
		return APP_FAIL;
	}

	g_is_configured = true;
	return APP_SUCCESS;
}

/******************************************************************************************
 * Function Name: mx25_open
 *
 * Description  : Open the QSPI driver.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 ******************************************************************************************/
app_err_t mx25_open(void)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_is_configured)
		result = APP_ERR_DRV_CONFIGURE;

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: mx25_close
 *
 * Description  : Close the QSPI driver.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 ******************************************************************************************/
app_err_t mx25_close(void)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_is_configured)
		result = APP_ERR_DRV_CONFIGURE;

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: mx25_write
 *
 * Description  : Writes bytes to the given address.
 *
 * Arguments    : uint32_t address, const uint8_t *p_buffer, uint32_t length
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_MX25_BUSY             MX25 is busy.
 ******************************************************************************************/
app_err_t mx25_write(uint32_t address, uint8_t* p_buffer, uint32_t length)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else if (p_buffer == NULL)
		result = APP_ERR_ASSERTION;
	else if ((address + length) > MACRONIX_CHIP_SIZE)
		result = APP_ERR_ASSERTION;
	else
	{
		FILE* fp = fopen(SIM_EXT_FLASH_FILENAME, "r+b");
		if (fp)
		{
			if (fseek(fp, address, SEEK_SET) == 0)
			{
				if (fwrite(p_buffer, 1, length, fp) == length)
					result = APP_SUCCESS;

			}
			fclose(fp);
		}
	}

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: mx25_read
 *
 * Description  : Reads certain bytes from the given address.
 *
 * Arguments    : uint32_t address, uint8_t *p_buffer, uint32_t length
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_MX25_BUSY             MX25 is busy.
 ******************************************************************************************/
app_err_t mx25_read(uint32_t address, uint8_t* p_buffer, uint32_t length)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else if (p_buffer == NULL)
		result = APP_ERR_ASSERTION;
	else if ((address + length) > MACRONIX_CHIP_SIZE)
		result = APP_ERR_ASSERTION;
	else
	{
		FILE* fp = fopen(SIM_EXT_FLASH_FILENAME, "rb");
		if (fp)
		{
			if (fseek(fp, address, SEEK_SET) == 0)
			{
				if (fread(p_buffer, 1, length, fp) == length)
					result = APP_SUCCESS;
			}
			fclose(fp);
		}
	}

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: mx25_sector_erase
 *
 * Description  : Erase log data from the external flash. Erase 1 block (64Kb) data.
 *                Takes max time 1 second.
 *
 * Arguments    : uint32_t address
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_THREADX               An unexpected error occured on ThreadX.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_MX25_BUSY             MX25 is busy.
 ******************************************************************************************/
app_err_t mx25_sector_erase(uint32_t address)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_FAIL;

	if (!g_is_configured)
		result = APP_ERR_DRV_CONFIGURE;
	else
		internal_file_erase(address, MACRONIX_SECTOR_SIZE) ? (result = APP_SUCCESS) : (result = APP_FAIL);

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

/******************************************************************************************
 * Function Name: mx25_space_erase
 *
 * Description  : Deletes flash specified space using ext_flash_4kb_erase function.
 *
 * Arguments    : uint32_t startAddress, uint32_t stopAddress
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 ******************************************************************************************/
app_err_t mx25_space_erase(uint32_t startAddress, uint32_t stopAddress)
{
	UINT tx_res = tx_mutex_get(&mx25_rdy_lock, TX_WAIT_FOREVER);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	app_err_t result = APP_SUCCESS;

	if (!g_is_configured) 
		result = APP_ERR_DRV_CONFIGURE;
	else if (stopAddress <= startAddress) 
		result = APP_ERR_ASSERTION;
	else if (stopAddress > MACRONIX_CHIP_SIZE) 
		result = APP_ERR_ASSERTION;
	else
	{
		// Erase in 4KB sector increments.
		for (uint32_t addr = startAddress; addr < stopAddress; addr += MACRONIX_SECTOR_SIZE)
		{
			if (!internal_file_erase(addr, MACRONIX_SECTOR_SIZE))
			{
				result = APP_FAIL;
				break; // Stop on the first error
			}
		}
	}

	tx_res = tx_mutex_put(&mx25_rdy_lock);
	if (TX_SUCCESS != tx_res)
		return APP_ERR_THREADX;

	return result;
}

#endif
