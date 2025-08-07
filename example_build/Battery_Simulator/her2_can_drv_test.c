
/*------------------------------------ Includes ------------------------------------------*/

#include "her2_can_drv_test.h"

#if(CANBUS_TEST)
#include "her2_can_drv.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ---------------------------------*/

#define CAN_FRAME_SEND_TIMEOUT         100      // 100ms

/*------------------------------------ Static Variables ----------------------------------*/

static app_can_frame_t can_recv_packet = { 0 };

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

//static void can_rx_event(app_can_frame_t* p_packet);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: can_rx_event
 *
 * Description  : Handles the incoming CAN Bus packet event. It write to same message to
 * CAN Bus.
 *
 * Arguments    : can_frame_t *p_packet
 *
 * Return Value : None
 ******************************************************************************************/
static void can_rx_event(app_can_frame_t* p_packet)
{
	can_recv_packet.id = p_packet->id;
	can_recv_packet.type = p_packet->type;
	can_recv_packet.data_length_code = p_packet->data_length_code;

	for (uint8_t i = 0; i < 8; i++)
		can_recv_packet.data[i] = (uint8_t)(p_packet->data[i]);

}

/*----------------------------------- Public Functions -----------------------------------*/

/******************************************************************************************
 * Function Name: can_app_init
 *
 * Description  : Initialize application layer of the CAN Bus module.
 *
 * Arguments    : None
 *
 * Return Value : bool
 ******************************************************************************************/
void can_test_init(void)
{
	can_drv_configure(&can_rx_event, NULL);
}

/******************************************************************************************
 * Function Name: can_app_test
 *
 * Description  : Test method of CAN Bus module. Do basic operations to test if CAN Bus
 *                is working as expected.
 *
 * Arguments    : None
 *
 * Return Value : None
 ******************************************************************************************/
void can_test_main(void)
{
	printf("\nCAN ID: %lu", can_recv_packet.id);
	printf("\nCAN DLC: %d", can_recv_packet.data_length_code);
	printf("\nCAN DATA: %d", can_recv_packet.data[0]);
	printf("\n");
}

#endif