#include <stdio.h>

#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
  #include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
  #define LED_DELAY_MS 500
#endif

int pico_led_init(void) {
#if defined(CYW43_WL_GPIO_LED_PIN)
  return cyw43_arch_init();
#endif
}

void pico_set_led(bool led_on) {
#if defined(CYW43_WL_GPIO_LED_PIN)
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// Tässä on vähän ylimääräistä serialmonitorin debuggausta varten
int main() {
  stdio_init_all();
  int rc = pico_led_init();
  hard_assert(rc == PICO_OK);
  while (true) {
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
    printf("Hello, Pico!\n");
  }
}
