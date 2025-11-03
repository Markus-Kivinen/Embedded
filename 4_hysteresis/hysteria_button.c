#include <stdio.h>

#include "hardware/pwm.h"
#include "input_hysteresis.h"
#include "pico/stdlib.h"

#define PWM_DUTY_CYCLE_PERCENT 25u
#define PWM_WRAP 9u
#define LED_PIN 3
#define BUTTON_PIN 9

volatile int led_mode = 0;

static void disable_pwm_for_pin(int gpio_pin) {
  int slice_num = pwm_gpio_to_slice_num(gpio_pin);
  pwm_set_enabled(slice_num, false);
  gpio_set_function(gpio_pin, GPIO_FUNC_SIO);
}

// https://github.com/raspberrypi/pico-examples/blob/master/pwm/hello_pwm/hello_pwm.c#L14-L29
static void configure_pwm_for_pin(int gpio_pin, int duty_percent) {
  gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
  int slice_num = pwm_gpio_to_slice_num(gpio_pin);
  pwm_set_wrap(slice_num, PWM_WRAP);
  int level = ((int)(PWM_WRAP + 1) * duty_percent) / 100u;
  pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), level);
  pwm_set_enabled(slice_num, true);
}

void button_callback(unsigned int gpio, uint32_t events) {
  if (hysteresis_read_debounced(BUTTON_PIN, 200)) {
    led_mode = (led_mode + 1) % 4;
    if (led_mode == 1) {
      printf("LED Mode: ON\n");
      gpio_put(LED_PIN, 1);
    }
    else if (led_mode == 2) {
      printf("LED Mode: DIM\n");
      configure_pwm_for_pin(LED_PIN, PWM_DUTY_CYCLE_PERCENT);
    }
    else if (led_mode == 3) {
      printf("LED Mode: BLINK\n");
      disable_pwm_for_pin(LED_PIN);
    }
    else {
      printf("LED Mode: OFF\n");
      gpio_put(LED_PIN, 0);
    }
  }
}

int main(void) {
  stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  hysteresis_setup(BUTTON_PIN);

  if (hysteresis_is_enabled(BUTTON_PIN)) {
    printf("Hysteresis enabled on button pin %d\n", BUTTON_PIN);
  }
  else {
    printf("Hysteresis NOT enabled on button pin %d, crashing program\n", BUTTON_PIN);
    return PICO_ERROR_GENERIC;
  }
  gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

  while (1) {
    if (led_mode == 3) {
      gpio_put(LED_PIN, 1);
      sleep_ms(500);
      gpio_put(LED_PIN, 0);
      sleep_ms(500);
    }
    else {
      sleep_ms(1000);
    }
  }
  return 0;
}