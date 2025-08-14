/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : jsn_log.h
 * Author       : Gokhan Akkaya
 * Description  : Common logger module.
 * Project      : VESTEL Common modules project
 * Creation Date: 15.04.2022
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_log.h"

#if(COMMON_LOGGER)
#include "her2_rtc_drv.h"
#include "jsn_utils.h"
#endif

/*----------------------------- Private Constant & Macro ----------------------------------*/

#if(COMMON_LOGGER)
#define CHECK_WRONG_FRAME_AREA(_x)			((0 == _x) || (_x >= LOGGER_NUMBER))
#define LOG_INFO_BUFF_SIZE					24
#endif

#define LOG_SW_LOGFILE_NAME					"simulation_log.txt"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

#if(COMMON_LOGGER)
typedef struct
{
	bool log_flash;
	bool log_monitor;
	bool log_time;
} log_t;
#endif

/*------------------------------------ Static Variables ----------------------------------*/

#if(COMMON_LOGGER)
static FILE* g_log_file = NULL;
static TX_MUTEX common_log_mutex;
static log_t logger[LOGGER_NUMBER];
#endif

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Public Functions -----------------------------------*/

#if(COMMON_LOGGER)
/******************************************************************************************
 * Function Name: log_init
 *
 * Description  : Takes a pointer to a function, and sets it as the callback
 *                function for the initialize logger.
 *
 * Arguments    : pointer to callback function (set to NULL to disable)
 *
 * Return Value : bool
 *******************************************************************************************/
bool log_init(void)
{
	if (g_log_file != NULL) 
		fclose(g_log_file);
	
#if(LOGGER_PLOG_FLASH)
	uint8_t logIndSize = 0;
#endif

#if(LOGGER_PROGRAM_LOG)
	logger[LOGGER_PLOG_INDEX].log_flash = (bool)LOGGER_PLOG_FLASH;
	logger[LOGGER_PLOG_INDEX].log_monitor = (bool)LOGGER_PLOG_MONITOR;
	logger[LOGGER_PLOG_INDEX].log_time = (bool)LOGGER_PLOG_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_PLOG_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_PLOG_INDEX, LOGGER_PLOG_START_OFFSET, LOGGER_PLOG_SIZE, logIndSize, LOGGER_PLOG_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_1)
	logger[LOGGER_FRAME1_INDEX].log_flash = (bool)LOGGER_FRAME1_FLASH;
	logger[LOGGER_FRAME1_INDEX].log_monitor = (bool)LOGGER_FRAME1_MONITOR;
	logger[LOGGER_FRAME1_INDEX].log_time = (bool)LOGGER_FRAME1_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME1_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME1_INDEX, LOGGER_FRAME1_START_OFFSET, LOGGER_FRAME1_SIZE, logIndSize, LOGGER_FRAME1_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_2)
	logger[LOGGER_FRAME2_INDEX].log_flash = (bool)LOGGER_FRAME2_FLASH;
	logger[LOGGER_FRAME2_INDEX].log_monitor = (bool)LOGGER_FRAME2_MONITOR;
	logger[LOGGER_FRAME2_INDEX].log_time = (bool)LOGGER_FRAME2_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME2_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME2_INDEX, LOGGER_FRAME2_START_OFFSET, LOGGER_FRAME2_SIZE, logIndSize, LOGGER_FRAME2_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_3)
	logger[LOGGER_FRAME3_INDEX].log_flash = (bool)LOGGER_FRAME3_FLASH;
	logger[LOGGER_FRAME3_INDEX].log_monitor = (bool)LOGGER_FRAME3_MONITOR;
	logger[LOGGER_FRAME3_INDEX].log_time = (bool)LOGGER_FRAME3_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME3_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME3_INDEX, LOGGER_FRAME3_START_OFFSET, LOGGER_FRAME3_SIZE, logIndSize, LOGGER_FRAME3_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_4)
	logger[LOGGER_FRAME4_INDEX].log_flash = (bool)LOGGER_FRAME4_FLASH;
	logger[LOGGER_FRAME4_INDEX].log_monitor = (bool)LOGGER_FRAME4_MONITOR;
	logger[LOGGER_FRAME4_INDEX].log_time = (bool)LOGGER_FRAME4_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME4_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME4_INDEX, LOGGER_FRAME4_START_OFFSET, LOGGER_FRAME4_SIZE, logIndSize, LOGGER_FRAME4_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_5)
	logger[LOGGER_FRAME5_INDEX].log_flash = (bool)LOGGER_FRAME5_FLASH;
	logger[LOGGER_FRAME5_INDEX].log_monitor = (bool)LOGGER_FRAME5_MONITOR;
	logger[LOGGER_FRAME5_INDEX].log_time = (bool)LOGGER_FRAME5_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME5_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME5_INDEX, LOGGER_FRAME5_START_OFFSET, LOGGER_FRAME5_SIZE, logIndSize, LOGGER_FRAME5_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_6)
	logger[LOGGER_FRAME6_INDEX].log_flash = (bool)LOGGER_FRAME6_FLASH;
	logger[LOGGER_FRAME6_INDEX].log_monitor = (bool)LOGGER_FRAME6_MONITOR;
	logger[LOGGER_FRAME6_INDEX].log_time = (bool)LOGGER_FRAME6_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME6_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME6_INDEX, LOGGER_FRAME6_START_OFFSET, LOGGER_FRAME6_SIZE, logIndSize, LOGGER_FRAME6_OVERWRITE);
#endif
#endif

#if(LOGGER_FRAME_LOG_7)
	logger[LOGGER_FRAME7_INDEX].log_flash = (bool)LOGGER_FRAME7_FLASH;
	logger[LOGGER_FRAME7_INDEX].log_monitor = (bool)LOGGER_FRAME7_MONITOR;
	logger[LOGGER_FRAME7_INDEX].log_time = (bool)LOGGER_FRAME7_TIME;
#if(LOGGER_PLOG_FLASH)
	(logger[LOGGER_FRAME7_INDEX].log_time) ? (logIndSize = LOG_IND_SIZE_TIME) : (logIndSize = LOG_IND_SIZE);
	flash_log_init(LOGGER_FRAME7_INDEX, LOGGER_FRAME7_START_OFFSET, LOGGER_FRAME7_SIZE, logIndSize, LOGGER_FRAME7_OVERWRITE);
#endif
#endif

#if(LOGGER_MONITOR)
	//monitor_log_init(); // TODO GA: Open it later.
#endif

	errno_t err = fopen_s(&g_log_file, LOG_SW_LOGFILE_NAME, "a");

	if (err != 0 || g_log_file == NULL) 
	{
		perror("ERROR: Could not open log file");
		return false;
	}
	fprintf(g_log_file, "--- LOG SESSION STARTED ---\n");
	fflush(g_log_file);

	UINT err_log_mutex = tx_mutex_create(&common_log_mutex, (CHAR*)"Common Log Mutex", TX_INHERIT);
	if (TX_SUCCESS != err_log_mutex)
	{
		return false;
	}

	return true;
}

bool log_set_target(uint8_t log_area, log_target_e target, bool status)
{
	tx_mutex_get(&common_log_mutex, TX_WAIT_FOREVER);

	bool res = false;
	if (log_area < LOGGER_NUMBER)
	{
		if (log_monitor == target)
		{
			logger[log_area].log_monitor = status;
			res = true;
		}
#if(LOGGER_PLOG_FLASH)
		else if (log_flash == target)
		{
			logger[log_area].log_flash = status;
			res = true;
		}
#endif
	}

	tx_mutex_put(&common_log_mutex);
	return res;
}

/******************************************************************************************
 * Function Name: log_sw
 *
 * Description  : it is used for writing text logs to log target in queue.
 *
 * Arguments    : uint8_t *text,
 *                uint16_t code,
 *                log_verbose_e verbose
 *
 * Return Value : None
 ******************************************************************************************/
void log_sw(uint8_t* text, uint32_t code, log_verbose_e verbose)
{
	tx_mutex_get(&common_log_mutex, TX_WAIT_FOREVER);

	if (g_log_file == NULL )
		return;

	uint8_t logBuff[LOG_QUEUE_SIZE] = { 0 };

	if (false == logger[LOGGER_PLOG_INDEX].log_time)
	{
#if(PURE_LOG)
		uint8_t codeBuff[LOG_QUEUE_SIZE - LOG_IND_SIZE] =
		{ 0 };
		snprintf((char*)codeBuff, (LOG_QUEUE_SIZE - LOG_IND_SIZE - 2), "%s\r\n", text);
		jsn_strncat2((char*)(&logBuff[LOG_IND_SIZE]), (char*)codeBuff, (uint8_t)(LOG_QUEUE_SIZE - LOG_IND_SIZE));
#else
		uint8_t codeBuff[LOG_INFO_BUFF_SIZE] =
		{ 0 };
		jsn_strncat(&logBuff[LOG_IND_SIZE], text,
			(LOG_QUEUE_SIZE - LOG_INFO_BUFF_SIZE - LOG_IND_SIZE));
		snprintf((char*)codeBuff, LOG_INFO_BUFF_SIZE, ",type=%d,code=%lu\r\n", verbose, code);
		jsn_strncat2((char*)(&logBuff[LOG_IND_SIZE]), (char*)codeBuff, (uint8_t)LOG_INFO_BUFF_SIZE);
#endif

		size_t log_size = (jsn_strlen(&logBuff[LOG_IND_SIZE])) + LOG_IND_SIZE; // indicators(verbose and log_size)
		logBuff[0] = (uint8_t)verbose;
		logBuff[1] = (uint8_t)log_size;
		logBuff[2] = (uint8_t)LOGGER_PLOG_INDEX;
	}
	else
	{
		uint8_t codeBuff[LOG_INFO_BUFF_SIZE] =
		{ 0 };
		struct tm temp_time;
		rtc_drv_time_get(&temp_time);

		jsn_strncat(&logBuff[LOG_IND_SIZE_TIME], text, (LOG_QUEUE_SIZE - LOG_INFO_BUFF_SIZE - LOG_IND_SIZE_TIME));
		snprintf((char*)codeBuff, LOG_INFO_BUFF_SIZE, ",type=%d,code=%lu\r\n", verbose, code);
		jsn_strncat(&logBuff[LOG_IND_SIZE_TIME], codeBuff, LOG_INFO_BUFF_SIZE);
		size_t log_size = (jsn_strlen(&logBuff[LOG_IND_SIZE_TIME])) + LOG_IND_SIZE_TIME; // indicators(verbose, log_size and time)
		logBuff[0] = (uint8_t)verbose;
		logBuff[1] = (uint8_t)log_size;
		logBuff[2] = (uint8_t)LOGGER_PLOG_INDEX;           // Since it's only the program log printed as char
		logBuff[3] = (uint8_t)(temp_time.tm_year - 100);   // year(data+2000)
		logBuff[4] = (uint8_t)(temp_time.tm_mon + 1);      // month(1-12)
		logBuff[5] = (uint8_t)temp_time.tm_mday;           // day(1-31)
		logBuff[6] = (uint8_t)temp_time.tm_hour;           // hour(00-23)
		logBuff[7] = (uint8_t)temp_time.tm_min;            // min(00-59)
		logBuff[8] = (uint8_t)temp_time.tm_sec;            // second(00-59)
	}

	size_t total_log_size = (size_t)logBuff[1];
	fwrite(&logBuff[2], sizeof(char), total_log_size, g_log_file);

	tx_mutex_put(&common_log_mutex);
}

/******************************************************************************************
 * Function Name: log_frame
 *
 * Description  : it is used for writing frame logs to flash in queue.
 *
 * Arguments    : uint8_t log_area,
 *                uint8_t *arr,
 *                uint8_t data_len
 *
 * Return Value : None
 ******************************************************************************************/
void log_frame(uint8_t log_area, uint8_t* arr, uint8_t data_len)
{
	tx_mutex_get(&common_log_mutex, TX_WAIT_FOREVER);

	bool is_wrong_area = CHECK_WRONG_FRAME_AREA(log_area);

	if (!is_wrong_area)       //except for program log.
	{
		if (false == logger[log_area].log_time)
		{
			if (data_len <= (LOG_QUEUE_SIZE - LOG_IND_SIZE))
			{
				uint8_t logBuff[LOG_QUEUE_SIZE] =
				{ 0 };
				logBuff[0] = (uint8_t)log_data_frame;
				logBuff[1] = (uint8_t)(data_len + LOG_IND_SIZE);
				logBuff[2] = (uint8_t)log_area;

				for (uint8_t ind = 0; ind < data_len; ind++)
					logBuff[ind + LOG_IND_SIZE] = arr[ind];
				log_write(logBuff);
			}
		}
		else
		{
			if (data_len <= (LOG_QUEUE_SIZE - LOG_IND_SIZE_TIME))
			{
				uint8_t logBuff[LOG_QUEUE_SIZE] =
				{ 0 };
				struct tm temp_time;
				rtc_drv_time_get(&temp_time);

				logBuff[0] = (uint8_t)log_data_frame;
				logBuff[1] = (uint8_t)(data_len + LOG_IND_SIZE_TIME);
				logBuff[2] = (uint8_t)log_area;
				logBuff[3] = (uint8_t)(temp_time.tm_year - 100);   // year(data+2000)
				logBuff[4] = (uint8_t)(temp_time.tm_mon + 1);      // month(1-12)
				logBuff[5] = (uint8_t)temp_time.tm_mday;           // day(1-31)
				logBuff[6] = (uint8_t)temp_time.tm_hour;           // hour(00-23)
				logBuff[7] = (uint8_t)temp_time.tm_min;            // min(00-59)
				logBuff[8] = (uint8_t)temp_time.tm_sec;            // second(00-59)

				for (uint8_t ind = 0; ind < data_len; ind++)
					logBuff[ind + LOG_IND_SIZE_TIME] = arr[ind];

				log_write(logBuff);
			}
		}
	}
	tx_mutex_put(&common_log_mutex);
}

void log_write(uint8_t* p_log_str)
{
	tx_mutex_get(&common_log_mutex, TX_WAIT_FOREVER);

	uint8_t log_index = p_log_str[LOG_IND_INDEX_LOC];
	if (LOGGER_NUMBER > log_index)       //except for program log.
	{

#if(LOGGER_PLOG_FLASH)
		if (logger[log_index].log_flash)
			flash_log_send(p_log_str);
#endif

#if(LOGGER_MONITOR)
		if (logger[log_index].log_monitor)
		{
			//monitor_log_send(p_log_str);	// TODO GA: Open it later.
		}
#endif
	}
	tx_mutex_put(&common_log_mutex);
}

#endif