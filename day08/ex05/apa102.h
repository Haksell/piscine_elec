#pragma once

#include <avr/io.h>
#include <stdlib.h>

#define NUM_LEDS 3
#define BRIGHTNESS 1

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_rgb;

void apa102_init();
void apa102_send_colors(t_rgb colors[NUM_LEDS]);
