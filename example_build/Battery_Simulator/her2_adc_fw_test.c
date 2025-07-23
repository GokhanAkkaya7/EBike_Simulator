
#include "her2_adc_fw.h"

#define TEST_ARRAY_SIZE				72

#if(ADC_TEST)

uint8_t test_index; 
uint16_t p_test_buffer[TEST_ARRAY_SIZE];


void adc_event_test(uint8_t index, uint16_t* p_buffer)
{
	test_index = index;
	for (uint8_t adc_array_index = 0; adc_array_index < TEST_ARRAY_SIZE; adc_array_index++)
	{
		p_test_buffer[adc_array_index] = p_buffer[adc_array_index];
	}
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