/******************************************************************************************
 * © Copyright [2021-2023] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her_dbc.c
 * Author       : Gokhan Akkaya
 * Description  : Debouncer module.
 * Project      : VESTEL Common modules project
 * Creation Date: 25.03.2022
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "jsn_dbc.h"
#if(DBC_APP)
#include "conf_dbc.h"
#include "jsn_threadx_utils.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

/*----------------------------- Private Constant & Macro ----------------------------------*/

/*------------------------------------ Static Variables ----------------------------------*/

static dbc_irq_t* dbc0;

/*---------------------------------- Global Variables ------------------------------------*/

/*------------------------------- Private Function Prototypes ----------------------------*/

/*----------------------------------- Private Functions ----------------------------------*/

/*----------------------------------- Global Functions ----------------------------------*/

#if(DBC_CH0)
/******************************************************************************************
 * Function Name: dbc0_flag_set
 * Description  : Sets the callback function of the ch0 interrupt as dbc_pinctrl_start.
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc0_flag_set(void)
{
	dbc0->dbc_irq_event();
}

/******************************************************************************************
 * Function Name: dbc0_configure
 * Description  : Configure dbc0.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc0_configure(dbc_irq_t* p_dbc_irq)
{
	dbc0 = p_dbc_irq;
	return true;
}
#endif

#if(DBC_CH1)

/******************************************************************************************
 * Function Name: dbc1_flag_set
 * Description  : Sets the callback function of the ch1 interrupt as dbc_pinctrl_start .
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc1_flag_set()
{
}
/******************************************************************************************
 * Function Name: dbc1_configure
 * Description  : Configure dbc1.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc1_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}

#endif

#if(DBC_CH2)
/******************************************************************************************
 * Function Name: dbc2_flag_set
 * Description  : Sets the callback function of the ch2 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc2_flag_set()
{

}

/******************************************************************************************
 * Function Name: dbc2_configure
 * Description  : Configure dbc2.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc2_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}
#endif

#if(DBC_CH3)
/******************************************************************************************
 * Function Name: dbc3_flag_set
 * Description  : Sets the callback function of the ch3 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc3_flag_set()
{
}

/******************************************************************************************
 * Function Name: dbc3_configure
 * Description  : Configure dbc3.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc3_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}
#endif

#if(DBC_CH4)
/******************************************************************************************
 * Function Name: dbc4_flag_set
 * Description  : Sets the callback function of the ch4 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc4_flag_set()
{
}

/******************************************************************************************
 * Function Name: dbc4_configure
 * Description  : Configure dbc4.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc4_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}
#endif

#if(DBC_CH5)
/******************************************************************************************
 * Function Name: dbc5_flag_set
 * Description  : Sets the callback function of the ch5 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc5_flag_set()
{
}
/******************************************************************************************
 * Function Name: dbc5_configure
 * Description  : Configure dbc5.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc5_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}

#endif

#if(DBC_CH6)
/******************************************************************************************
 * Function Name: dbc6_flag_set
 * Description  : Sets the callback function of the ch6 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc6_flag_set()
{
}

/******************************************************************************************
 * Function Name: dbc6_configure
 * Description  : Configure dbc6.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc6_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}

#endif

#if(DBC_CH7)
/******************************************************************************************
 * Function Name: dbc7_flag_set
 * Description  : Sets the callback function of the ch7 interrupt as dbc_pinctrl_start .
 * Log_Code     : -
 * Arguments    : None
 * Return Value : None
 ******************************************************************************************/
void dbc7_flag_set()
{
}

/******************************************************************************************
 * Function Name: dbc7_configure
 * Description  : Configure dbc7.
 * Arguments    : p_dbc_irq* : Pointer to the debouncer struct.
 * Return Value : bool
 ******************************************************************************************/
bool dbc7_configure(dbc_irq_t* p_dbc_irq)
{
	return true;
}

#endif
#endif
