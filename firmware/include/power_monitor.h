#pragma once

#include <stdint.h>

void power_monitor_init();
uint16_t power_monitor_vdd_mv();
bool power_is_stable();
