#pragma once

/*------------------------------------ Includes ------------------------------------------*/

#include "conf_project_settings.h"

#if(IRQ_DRV)
#include "conf_irq_drv.h"
#include "driver_api.h"


/*-------------------------------- Public  Macro Defines ---------------------------------*/

/*----------------------------- Public Typedef Definitions -------------------------------*/

/* Trigger type: rising edge, falling edge, both edges, low level. */
typedef enum
{
    EXT_IRQ_TRIG_FALLING = 0,      // Falling edge trigger
    EXT_IRQ_TRIG_RISING = 1,       // Rising edge trigger
    EXT_IRQ_TRIG_BOTH_EDGE = 2,    // Both edges trigger
    EXT_IRQ_TRIG_LEVEL_LOW = 3,    // Low level trigger
} app_external_irq_trigger_t;

/*----------------------------- Public Function Prototypes -------------------------------*/

app_err_t irq_init(void);
void IRQ_CALLBACK(IrqData* incoming_irq_data);

#if(IRQ_CH0)
void irq_drv_configure0(void (*p_irq_ch0_event)(void));
app_err_t irq_open_ch0(void);
app_err_t irq_close_ch0(void);
app_err_t irq_set_trigger_ch0(app_external_irq_trigger_t p_irq0_trg);
app_err_t irq_get_trigger_ch0(app_external_irq_trigger_t* p_irq0_trg);
#endif

#endif