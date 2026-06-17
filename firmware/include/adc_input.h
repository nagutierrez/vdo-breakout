#pragma once

#include <stdint.h>

void adc_input_init();
void adc_input_update();

uint16_t adc_filtered_raw();
uint8_t adc_brightness();
