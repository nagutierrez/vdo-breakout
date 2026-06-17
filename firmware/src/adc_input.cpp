#include "adc_input.h"

#include <Arduino.h>

#include "config.h"

static uint16_t filtered_adc = 0;

void adc_input_init() {
  pinMode(PIN_ADC, INPUT);
  analogReference(VDD);
  filtered_adc = analogRead(PIN_ADC);
}

void adc_input_update() {
  const uint16_t raw = analogRead(PIN_ADC);
  filtered_adc -= filtered_adc >> ADC_EMA_SHIFT;
  filtered_adc += raw >> ADC_EMA_SHIFT;
}

uint16_t adc_filtered_raw() { return filtered_adc; }

uint8_t adc_brightness() {
  if (filtered_adc <= ADC_MIN) {
    return 0;
  }
  if (filtered_adc >= ADC_MAX) {
    return 255;
  }

  return map(filtered_adc, ADC_MIN, ADC_MAX, 0, 255);
}
