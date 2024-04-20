#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/twi.h>

#define MIN(A, B) ((A < B) ? (A) : (B))
#define MAX(A, B) ((A > B) ? (A) : (B))

#include "aht20.h"
#include "i2c.h"
#include "uart.h"
