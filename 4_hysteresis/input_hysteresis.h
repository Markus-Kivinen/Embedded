#ifndef INPUT_HYSTERESIS_H
#define INPUT_HYSTERESIS_H

#include <stdbool.h>
#include <stdint.h>

void hysteresis_setup(unsigned int gpio_pin);
bool hysteresis_is_enabled(unsigned int gpio_pin);
bool hysteresis_read_debounced(unsigned int gpio_pin, unsigned int debounce_ms);

#endif