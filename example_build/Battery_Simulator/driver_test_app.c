#include "driver_test_app.h"

#include "her2_adc_fw_test.h"
#include "her2_can_drv_test.h"
#include "her2_irq_drv_test.h"
#include "her2_gpt_drv_test.h"
#include "her2_io_drv_test.h"
#include "her2_flash_drv_test.h"
#include "her2_mx25_spi_fw_test.h"
#include "her2_rtc_drv_test.h"

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

#if(GPT_TEST)

	gpt_test_init();
	gpt_test_main();

#endif

#if(IO_TEST)

	io_test_init();
	io_test_main();

#endif

#if(DFLASH_TEST || CFLASH_TEST)

	flash_test_init();
	flash_test_main();

#endif

#if(SPI_TEST)

	spi_test_init();
	spi_test_main();

#endif

#if(RTC_TEST)

	rtc_test_init();
	rtc_test_main();

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