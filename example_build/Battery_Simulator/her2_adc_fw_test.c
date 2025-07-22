
#include "her2_adc_fw.h"

#if(ADC_TEST)

uint8_t test_index; 
uint16_t* p_test_buffer;


void adc_event_test(uint8_t index, uint16_t* p_buffer)
{
	test_index = index;
	p_test_buffer = p_buffer;
}

void adc_test_init(void)
{
	adc0_fw_configure(adc_event_test);
}

void adc_test_main(void)
{
	printf("\nADC Index: %d", test_index);
	printf("\nADC Data: %d", p_test_buffer[0]);
	printf("\n");
}

#endif