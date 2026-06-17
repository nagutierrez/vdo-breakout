#pragma once

#include <Arduino.h>

// PA6: conditioned 0-5 V dimmer input. PA1: PWM to MOSFET gate. PA0: UPDI only.
constexpr uint8_t PIN_ADC = PIN_PA6;
constexpr uint8_t PIN_PWM = PIN_PA1;

// Hysteresis thresholds on 10-bit ADC counts (~2% / ~4% of full scale).
constexpr uint16_t ADC_OFF_THRESHOLD = 20;
constexpr uint16_t ADC_ON_THRESHOLD = 41;

// Map usable dimmer range to PWM duty; tune on bench if needed.
constexpr uint16_t ADC_MIN = 20;
constexpr uint16_t ADC_MAX = 1000;

constexpr uint8_t ADC_EMA_SHIFT = 4;

constexpr uint16_t SIGNAL_OFF_DEBOUNCE_MS = 100;
constexpr uint16_t LOOP_INTERVAL_MS = 20;

// Force PWM off when supply sags during capacitor discharge on power cut.
constexpr uint16_t VDD_SHUTDOWN_MV = 4500;

#ifndef LOAD_PROFILE_INCANDESCENT
#ifndef LOAD_PROFILE_LED
#define LOAD_PROFILE_INCANDESCENT
#endif
#endif

#if defined(LOAD_PROFILE_LED)
constexpr uint16_t FADE_IN_MS = 300;
constexpr uint16_t FADE_OUT_MS = 200;
#else
constexpr uint16_t FADE_IN_MS = 1500;
constexpr uint16_t FADE_OUT_MS = 500;
#endif
