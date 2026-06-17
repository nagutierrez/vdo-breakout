#include "fader.h"

#include <Arduino.h>

#include "config.h"
#include "pwm_output.h"

enum class FaderState : uint8_t { FadingIn, Active, FadingOut, Off };

static FaderState state = FaderState::FadingIn;
static uint8_t current_duty = 0;
static uint8_t fade_start_duty = 0;
static uint8_t fade_target_duty = 0;
static uint32_t fade_start_ms = 0;
static uint32_t signal_below_off_since_ms = 0;

static uint8_t scale_by_progress(const uint8_t value, const uint16_t progress) {
  return static_cast<uint8_t>((static_cast<uint16_t>(value) * progress) / 255);
}

static void begin_fade_in(const uint8_t target) {
  state = FaderState::FadingIn;
  fade_start_duty = 0;
  fade_target_duty = target;
  fade_start_ms = millis();
}

static void begin_fade_out() {
  state = FaderState::FadingOut;
  fade_start_duty = current_duty;
  fade_start_ms = millis();
}

static void apply_duty(const uint8_t duty) {
  current_duty = duty;
  pwm_output_set_duty(duty);
}

void fader_init() {
  state = FaderState::FadingIn;
  current_duty = 0;
  fade_start_duty = 0;
  fade_target_duty = 0;
  fade_start_ms = millis();
  signal_below_off_since_ms = 0;
  pwm_output_set_duty(0);
}

void fader_update(const uint8_t target_brightness, const uint16_t filtered_adc) {
  const uint32_t now = millis();

  if (filtered_adc < ADC_OFF_THRESHOLD) {
    if (signal_below_off_since_ms == 0) {
      signal_below_off_since_ms = now;
    }
  } else {
    signal_below_off_since_ms = 0;
  }

  const bool signal_off =
      filtered_adc < ADC_OFF_THRESHOLD &&
      signal_below_off_since_ms != 0 &&
      (now - signal_below_off_since_ms) >= SIGNAL_OFF_DEBOUNCE_MS;
  const bool signal_on = filtered_adc > ADC_ON_THRESHOLD;

  if (signal_on && (state == FaderState::FadingOut || state == FaderState::Off)) {
    begin_fade_in(target_brightness);
  } else if (signal_off && (state == FaderState::Active || state == FaderState::FadingIn)) {
    begin_fade_out();
  }

  switch (state) {
    case FaderState::FadingIn: {
      fade_target_duty = target_brightness;
      const uint32_t elapsed = now - fade_start_ms;
      uint16_t progress = 255;
      if (elapsed < FADE_IN_MS) {
        progress = static_cast<uint16_t>((elapsed * 255UL) / FADE_IN_MS);
      }

      const uint8_t span = fade_target_duty - fade_start_duty;
      const uint8_t duty = fade_start_duty + scale_by_progress(span, progress);
      apply_duty(duty);

      if (progress >= 255) {
        state = FaderState::Active;
      }
      break;
    }

    case FaderState::Active:
      apply_duty(target_brightness);
      break;

    case FaderState::FadingOut: {
      const uint32_t elapsed = now - fade_start_ms;
      uint16_t progress = 255;
      if (elapsed < FADE_OUT_MS) {
        progress = static_cast<uint16_t>((elapsed * 255UL) / FADE_OUT_MS);
      }

      const uint8_t duty = fade_start_duty - scale_by_progress(fade_start_duty, progress);
      apply_duty(duty);

      if (progress >= 255) {
        state = FaderState::Off;
      }
      break;
    }

    case FaderState::Off:
      apply_duty(0);
      break;
  }
}
