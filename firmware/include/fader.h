#pragma once

#include <stdint.h>

void fader_init();
void fader_update(uint8_t target_brightness, uint16_t filtered_adc);
