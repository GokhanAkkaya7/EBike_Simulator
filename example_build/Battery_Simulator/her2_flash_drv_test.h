#pragma once
#pragma once

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(DFLASH_TEST || CFLASH_TEST)

/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/*----------------------------- Public Function Prototypes -------------------------------*/

bool flash_test_init(void);
void flash_test_main(void);

#endif