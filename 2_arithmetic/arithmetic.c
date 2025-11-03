#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"

// Out of scope:
//  Yli 32 bitin syöte-/tulosluvut eivät ole tuettuja.
//  Desimaliluvut eivät ole tuettuja.
//  Jos on useampi operaattori putkeen vain ensimmäinen otetaan huomioon.

int main(void) {
  stdio_init_all(); // serial monitor
  int a, b;
  char operator;
  char line[100];

  while (1) {
    if (!fgets(line, sizeof(line), stdin)) {
      continue;
    }

    if (line[0] == 'q') {
      break;
    }

    int result = sscanf(line, "%d %c %d", &a, &operator, &b);

    if (result == 0 || result == EOF) {
      printf("Virheellinen syöte (tyhjä) kirjoita \"q\" lopettaaksesi ohjelman\n");
      continue;
    }
    else if (result < 3) {
      printf("Virheellinen syöte (puuttuvia argumentteja)\n");
      continue;
    }

    if (operator == '/' && b == 0) {
      printf("Nollalla jakaminen ei ole sallittua\n");
      continue;
    }

    int calc_result = 0;
    switch (operator) {
    case '+':
      calc_result = a + b;
      break;
    case '-':
      calc_result = a - b;
      break;
    case 'x':
    case '*':
      calc_result = a * b;
      break;
    case '/':
      calc_result = a / b;
      break;
    case '%':
      calc_result = a % b;
      break;
    default:
      printf("Tuntematon operaattori: %c\n", operator);
      continue;
    }

    printf("%d\n", calc_result);
  }

  return 0;
}