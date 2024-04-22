#pragma once

#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdbool.h>

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

#include "eeprom.h"
#include "string_utils.h"
#include "uart.h"
