#include "input_hysteresis.h"
#include "pico/stdlib.h"

#define MAX_GPIO_PINS 32
static uint32_t enabled_pins_bitmap = 0;

void hysteresis_setup(unsigned int gpio_pin) {
    if (gpio_pin < MAX_GPIO_PINS) {
        enabled_pins_bitmap |= (1u << gpio_pin);
    }
}

bool hysteresis_is_enabled(unsigned int gpio_pin) {
    if (gpio_pin < MAX_GPIO_PINS) {
        return (enabled_pins_bitmap & (1u << gpio_pin)) != 0;
    }
    return false;
}

bool hysteresis_read_debounced(unsigned int gpio_pin, unsigned int debounce_ms) {
    static uint32_t last_press_time[MAX_GPIO_PINS] = {0};
    uint32_t now = to_ms_since_boot(get_absolute_time());
    bool pressed = !gpio_get(gpio_pin);

    if (gpio_pin < MAX_GPIO_PINS && pressed && (now - last_press_time[gpio_pin] > debounce_ms)) {
        last_press_time[gpio_pin] = now;
        return true;
    }
    return false;
}