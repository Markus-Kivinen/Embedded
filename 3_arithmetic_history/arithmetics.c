#include "pico/stdlib.h" // serial monitor
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 10
#define HISTORY_LINE_LENGTH 26
#define INPUT_LINE_LENGTH 26

// Out of scope
//  Yli 32 bitin syöte-/tulosluvut eivät ole tuettuja.
//  Desimaliluvut eivät ole tuettuja.
//  Jos on useampi operaattori putkeen vain ensimmäinen otetaan huomioon.

int calculate(int a, char operator, int b, int *ok) {
    int calc_result = 0;
    *ok = 1;
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
        *ok = 0;
        return 0;
    }
    return (int)calc_result;
}

void print_history(char **history, int history_count) {
    if (history_count == 0) {
        printf("Komentohistoria on tyhjä.\n");
        return;
    }
    printf("Komentohistoria:\n");
    int count = history_count < HISTORY_SIZE ? history_count : HISTORY_SIZE;
    int start = history_count < HISTORY_SIZE ? 0 : history_count % HISTORY_SIZE;
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % HISTORY_SIZE;
        printf("%d: %s\n", i + 1, history[idx]);
    }
}

void clear_history(char **history, int *history_count) {
    *history_count = 0;
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i][0] = '\0';
    }
    printf("Historia tyhjennetty\n");
}

void show_calculation(char **history, int history_count, char *line) {
    int a, b, result;
    char operator;
    int cmd_number;
    result = sscanf(line + 1, "%d", &cmd_number);
    if (result != 1) {
        printf("Virheellinen syöte komennolle !\n");
        return;
    }
    int count = history_count < HISTORY_SIZE ? history_count : HISTORY_SIZE;
    int start = history_count < HISTORY_SIZE ? 0 : history_count % HISTORY_SIZE;
    if (cmd_number <= 0 || cmd_number > count) {
        printf("Tuntematon rivinumero: %d\n", cmd_number);
        return;
    }
    int idx = (start + (cmd_number - 1)) % HISTORY_SIZE;
    char *cmd = history[idx];
    result = sscanf(cmd, "%d %c %d", &a, &operator, &b);
    if (result != 3) {
        printf("Mitäs ihmettä, historia rivi on virheellinen: %s\n", cmd);
        return;
    }
    int ok;
    result = calculate(a, operator, b, &ok);
    // ok pitäisi olla tässä aina tosi
    printf("%s = %d\n", cmd, result);
}

int main(void) {
    stdio_init_all(); // serial monitor

    const char quit_command = 'q';
    const char history_command = 'h';
    const char clear_command = 'c';
    const char show_calculation_command = '!';

    // char history[HISTORY_SIZE][HISTORY_LINE_LENGTH] = {0};
    char **history = malloc(HISTORY_SIZE * sizeof(char *));
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i] = calloc(HISTORY_LINE_LENGTH, sizeof(char));
    }
    int history_count = 0;

    int a, b;
    char operator;
    char line[INPUT_LINE_LENGTH];

    while (1) {
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }

        if (line[0] == quit_command) {
            break;
        }
        else if (line[0] == show_calculation_command) {
            show_calculation(history, history_count, line);
            continue;
        }
        else if (line[0] == history_command) {
            print_history(history, history_count);
            continue;
        }
        else if (line[0] == clear_command) {
            clear_history(history, &history_count);
            continue;
        }

        int result = sscanf(line, "%d %c %d", &a, &operator, &b);

        if (result == 0 || result == EOF) {
            printf(
                "Virheellinen syöte (tyhjä) kirjoita %c lopettaaksesi ohjelman\n",
                quit_command);
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

        int ok;
        int calc_result = calculate(a, operator, b, &ok);
        if (!ok) {
            continue;
        }
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcpy(history[history_count % HISTORY_SIZE], line);
        history_count++;

        printf("%d\n", calc_result);
    }

    // Läpäisi valgrindin tietokoneella viimeeksi kun kokeilin
    for (size_t i = 0; i < HISTORY_SIZE; i++)
        free(history[i]);
    free(history);

    return 0;
}