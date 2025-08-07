/*------------------------------------ Includes ------------------------------------------*/

#include "her2_irq_drv.h"

#if(IRQ_DRV)

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

#define IRQ_CH(channel)						IRQ_CH##channel
#define irq_ch_event(channel)				irq_ch##channel##_event()

/*------------------------------------ Static Variables ----------------------------------*/

#if(IRQ_CH0)
static void (*irq_ch0_event)(void);
#endif

static TX_MUTEX irq_lock;
static TX_MUTEX* p_irq_lock = NULL;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static app_err_t irq_open_ch(void);
static app_err_t irq_close_ch(void);
static app_err_t irq_set_type(app_external_irq_trigger_t p_irq_trg);
static app_err_t irq_get_type(app_external_irq_trigger_t* p_irq_trg);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: irq_open_ch
 *
 * Description  : Open given IRQ Channel.
 *
 * Arguments    : const external_irq_instance_t *p_irq_drv
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 ******************************************************************************************/
static app_err_t irq_open_ch(void)
{
	UINT tx_res = tx_mutex_get(&irq_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&irq_lock);

	return result;
}

/******************************************************************************************
 * Function Name: irq_close_ch
 *
 * Description  : Close IRQ given Channel.
 *
 * Arguments    : const external_irq_instance_t *p_irq_drv
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t irq_close_ch(void)
{
	UINT tx_res = tx_mutex_get(&irq_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&irq_lock);

	return result;
}

/******************************************************************************************
 * Function Name: irq_set_type
 *
 * Description  : Sets the type of the given channel according to given settings.
 *
 * Arguments    : const external_irq_instance_t *p_irq_drv
 *                app_external_irq_trigger_t *p_irq_trg
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t irq_set_type(app_external_irq_trigger_t p_irq_trg)
{
	UINT tx_res = tx_mutex_get(&irq_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	//TODO GA:

	tx_res = tx_mutex_put(&irq_lock);

	return result;
}

/******************************************************************************************
 * Function Name: irq_get_type
 *
 * Description  : Gets the type of the trigger for given channel. Used for debouncing algorithm
 *
 * Arguments    : const external_irq_instance_t *p_irq_drv
 *                app_external_irq_trigger_t *p_irq_trg
 *
 * Return Value : APP_SUCCESS                   Operation successful.
 *                APP_ERR_DRV_CONFIGURE         Driver is not configured before.
 *                APP_ERR_SSP_ERR               Unexpected error on SSP layer.
 *                APP_ERR_THREADX               Unexpected error on ThreadX side.
 ******************************************************************************************/
static app_err_t irq_get_type(app_external_irq_trigger_t* p_irq_trg)
{
	UINT tx_res = tx_mutex_get(&irq_lock, TX_WAIT_FOREVER);

	app_err_t result = APP_SUCCESS;

	// TODO GA:

	tx_res = tx_mutex_put(&irq_lock);

	return result;
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: irq_init
 *
 * Description  : Initialize IRQ driver module, create mutex for locking operations.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 *******************************************************************************************/
app_err_t irq_init(void)
{
	if (NULL != p_irq_lock)
		return APP_SUCCESS;

	UINT tx_res = tx_mutex_create(&irq_lock, (CHAR*)"IRQ Driver Lock", TX_INHERIT);
	if (TX_SUCCESS != tx_res)
	{
		return APP_ERR_THREADX;
	}
	else
	{
		p_irq_lock = &irq_lock;
		return APP_SUCCESS;
	}
}

#if(IRQ_CH0)
/******************************************************************************************
 * Function Name: irq_drv_configure
 *
 * Description  : Configure IRQ Channel 0 Driver HAL. Call this method only once.
 *
 * Arguments    : void *p_irq_ch0_event(void)
 *
 * Return Value : None
 *******************************************************************************************/
void irq_drv_configure0(void (*p_irq_ch0_event)(void))
{
	irq_ch0_event = p_irq_ch0_event;
}

/******************************************************************************************
 * Function Name: irq_open_ch0
 *
 * Description  : Open IRQ Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 ******************************************************************************************/
app_err_t irq_open_ch0(void)
{
	return irq_open_ch();
}

/******************************************************************************************
 * Function Name: irq_close_ch0
 *
 * Description  : Close IRQ Channel 0.
 *
 * Arguments    : None
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 ******************************************************************************************/
app_err_t irq_close_ch0(void)
{
	return irq_close_ch();
}

/******************************************************************************************
 * Function Name: irq_set_trigger_ch0
 *
 * Description  : Sets the trigger type for IRQ Channel 0.
 *
 * Arguments    : app_external_irq_trigger_t irq0_trg
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 ******************************************************************************************/
app_err_t irq_set_trigger_ch0(app_external_irq_trigger_t irq0_trg)
{
	return irq_set_type(irq0_trg);
}

/******************************************************************************************
 * Function Name: irq_get_trigger_ch0
 *
 * Description  : Gets the trigger type for IRQ Channel 0.
 *
 * Arguments    : app_external_irq_trigger_t *p_irq0_trg
 *
 * Return Value : APP_SUCCESS                   operation successful
 *                APP_ERR_DRV_CONFIGURE         driver is not configured before
 *                APP_ERR_SSP_ERR               unexpected error on SSP layer
 *                APP_ERR_THREADX               unexpected error on ThreadX side
 ******************************************************************************************/
app_err_t irq_get_trigger_ch0(app_external_irq_trigger_t* p_irq0_trg)
{
	return irq_get_type(p_irq0_trg);
}

#endif

/******************************************************************************************
 * Function Name: IRQ_CALLBACK
 *
 * Description  : IRQ callback.
 *
 * Arguments    : IrqData* incoming_irq_data
 *
 * Return Value : None
 ******************************************************************************************/
void IRQ_CALLBACK(IrqData* incoming_irq_data)
{
	for (uint8_t callback_index = 0; callback_index < incoming_irq_data->channel_count; callback_index++)
	{		
		switch (incoming_irq_data->channel_list[callback_index])
		{
		case 0:
#if (IRQ_CH(0))
			irq_ch_event(0);
			break;
#endif
		case 1:
#if (IRQ_CH(1))
			irq_ch_event(1);
			break;
#endif
		case 2:
#if (IRQ_CH(2))
			irq_ch_event(2);
			break;
#endif
		case 3:
#if (IRQ_CH(3))
			irq_ch_event(3);
			break;
#endif
		case 4:
#if (IRQ_CH(4))
			irq_ch_event(4);
			break;
#endif
		case 5:
#if (IRQ_CH(5))
			irq_ch_event(5);
			break;
#endif
		case 6:
#if (IRQ_CH(6))
			irq_ch_event(6);
			break;
#endif
		case 7:
#if (IRQ_CH(7))
			irq_ch_event(7);
			break;
#endif
		case 8:
#if (IRQ_CH(8))
			irq_ch_event(8);
			break;
#endif
		case 9:
#if (IRQ_CH(9))
			irq_ch_event(9);
			break;
#endif
		case 10:
#if (IRQ_CH(10))
			irq_ch_event(10);
			break;
#endif
		case 11:
#if (IRQ_CH(11))
			irq_ch_event(11);
			break;
#endif
		case 12:
#if (IRQ_CH(12))
			irq_ch_event(12);
			break;
#endif
		case 13:
#if (IRQ_CH(13))
			irq_ch_event(13);
			break;
#endif
		case 14:
#if (IRQ_CH(14))
			irq_ch_event(14);
			break;
#endif
		case 15:
#if (IRQ_CH(15))
			irq_ch_event(15);
			break;
#endif
		case 16:
#if (IRQ_CH(16))
			irq_ch_event(16);
			break;
#endif
		case 17:
#if (IRQ_CH(17))
			irq_ch_event(17);
			break;
#endif
		case 18:
#if (IRQ_CH(18))
			irq_ch_event(18);
			break;
#endif
		case 19:
#if (IRQ_CH(19))
			irq_ch_event(19);
			break;
#endif
		case 20:
#if (IRQ_CH(20))
			irq_ch_event(20);
			break;
#endif
		default:
			break;
		}
	}
}
#endif