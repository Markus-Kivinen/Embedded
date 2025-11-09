#include "input_hysteresis.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BUTTON_PIN 0

// External LED
#define LED_PIN 2
#ifndef LED_PIN
// Pico 2W onboard LED
#  ifdef CYW43_WL_GPIO_LED_PIN
#    include "pico/cyw43_arch.h"
#    define LED_PIN CYW43_WL_GPIO_LED_PIN
// Default LED pin for other boards
#  elif defined(PICO_DEFAULT_LED_PIN)
#    define LED_PIN PICO_DEFAULT_LED_PIN
#  endif
#endif

volatile unsigned int button_count = 0;


void button_callback(unsigned int gpio, uint32_t events) {
  if (hysteresis_read_debounced(BUTTON_PIN, 200)) {
    button_count++;
    printf("Button pressed %d times\n", button_count);
  }
}

void setup(void) {
  stdio_init_all();
#if defined(CYW43_WL_GPIO_LED_PIN) && (LED_PIN == CYW43_WL_GPIO_LED_PIN)
  cyw43_arch_init();
#else
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
#endif

  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  hysteresis_setup(BUTTON_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
}

int main(void) {
  setup();
  unsigned int last_count = 0;
  bool led_state = false;
  while (true) {
    if (button_count != last_count) {
      last_count = button_count;
      led_state = true;
      #if defined(CYW43_WL_GPIO_LED_PIN) && (LED_PIN == CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(LED_PIN, true);
      #else
        gpio_put(LED_PIN, 1);
      #endif
    }
    else if (led_state) {
      led_state = false;
      #if defined(CYW43_WL_GPIO_LED_PIN) && (LED_PIN == CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(LED_PIN, false);
      #else
        gpio_put(LED_PIN, 0);
      #endif
    }
    sleep_ms(100);
  }
  return 0;
}