#pragma once
/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(IRQ_DRV)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

#define IRQ_CH0                             1

#if(IRQ_CH0)
#define IRQ0_DRV_NAME                       g_external_irq0
#endif

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

#endif