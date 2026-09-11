#pragma once
#include "stdint.h"

// returns battery percentage based on voltage in mV
uint8_t get_battery_percentage(uint16_t voltage);