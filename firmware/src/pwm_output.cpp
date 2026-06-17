#include "pwm_output.h"

#include <Arduino.h>

#include "config.h"

void pwm_output_init() { pinMode(PIN_PWM, OUTPUT); }

void pwm_output_set_duty(const uint8_t duty) { analogWrite(PIN_PWM, duty); }
