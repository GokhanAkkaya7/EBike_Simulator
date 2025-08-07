/*------------------------------------ Includes ------------------------------------------*/

#include "her2_irq_drv_test.h"

#if(IRQ_TEST)
#include "her2_irq_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

static volatile uint8_t irq_triggered_cnt = 0;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

static void irq_event(void);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: button_event
 *
 * Description  : Handles the incoming button event.
 *
 * Arguments    : uint8_t buffer_index
 *
 * Return Value : None
 ******************************************************************************************/
static void irq_event(void)
{
	irq_triggered_cnt++;
}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: irq_app_init
 *
 * Description  : Initialization of the IRQ module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
void irq_test_init(void)
{
	irq_drv_configure0(&irq_event);
}

/******************************************************************************************
 * Function Name: irq_app_test
 *
 * Description  : Test application of the IRQ module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
void irq_test_main(void)
{
	printf("\nIrq Triggered Count: %d", irq_triggered_cnt);
	printf("\n");
}

#endif
