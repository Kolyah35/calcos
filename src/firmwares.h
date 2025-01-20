#pragma once
#include <platform.h>
#include "blink500.h"
#include "blink1000.h"

uint8_t* const g_firmwares_bytes[] = {blink500, blink1000};
PGM_P g_firmwares_names[] = {"blink500.bin", "blink1000.bin"};