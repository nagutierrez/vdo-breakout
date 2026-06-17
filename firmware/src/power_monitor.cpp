#include "power_monitor.h"

#include <Arduino.h>

#include "config.h"

void power_monitor_init() {
  // Prime the internal reference path; dimmer ADC uses VDD reference afterward.
  power_monitor_vdd_mv();
}

uint16_t power_monitor_vdd_mv() {
#if MEGATINYCORE_SERIES == 2
  analogReference(INTERNAL1V024);
  analogReadEnh(ADC_VDDDIV10, 12);
  const int32_t raw = analogReadEnh(ADC_VDDDIV10, 12);
  analogReference(VDD);
  return static_cast<uint16_t>((raw * 10) >> 2);
#else
  analogReference(VDD);
  const uint8_t saved_vref = VREF.CTRLA;
  VREF.CTRLA = VREF_ADC0REFSEL_1V5_gc;
  analogRead(ADC_INTREF);
  const uint16_t reading = analogRead(ADC_INTREF);
  VREF.CTRLA = saved_vref;

  if (reading == 0) {
    return 0;
  }
  return static_cast<uint16_t>((1023UL * 1500UL) / reading);
#endif
}

bool power_is_stable() { return power_monitor_vdd_mv() >= VDD_SHUTDOWN_MV; }
