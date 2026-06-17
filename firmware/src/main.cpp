#include <Arduino.h>

#include "adc_input.h"
#include "config.h"
#include "fader.h"
#include "power_monitor.h"
#include "pwm_output.h"

void setup() {
  adc_input_init();
  power_monitor_init();
  pwm_output_init();
  fader_init();
}

void loop() {
  if (!power_is_stable()) {
    pwm_output_set_duty(0);
    delay(LOOP_INTERVAL_MS);
    return;
  }

  adc_input_update();
  fader_update(adc_brightness(), adc_filtered_raw());
  delay(LOOP_INTERVAL_MS);
}
