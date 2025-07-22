#include "driver_test_app.h"

#include "her2_adc_fw_test.h"
#include "her2_can_drv_test.h"
#include "her2_irq_drv_test.h"

#if(APP_TEST)

void test_app_init(void)
{

#if(ADC_TEST)

	adc_test_init();

#endif

#if(CANBUS_TEST)

	can_test_init();

#endif

#if(IRQ_TEST)

	irq_test_init();

#endif

}

void test_app_main(void)
{

#if(ADC_TEST)

	adc_test_main();

#endif

#if(CANBUS_TEST)

	can_test_main();

#endif

#if(IRQ_TEST)

	irq_test_main();

#endif

}

#endif