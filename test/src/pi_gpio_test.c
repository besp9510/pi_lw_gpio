// Include C standard libraries:
#include <stdlib.h> // C Standard library
#include <stdio.h>  // C Standard I/O libary
#include <time.h>   // C Standard date and time manipulation

// Include pi_gpio.c:
#include "pi_gpio.h"

// Read GPIO modes and levels and print in a table
static void read_gpio(void) {
    // Store values and modes:
    int read_gpio_values[40];
    int read_gpio_modes[40];

    int i;

    // Read all pins and save to revert later:
    for (i = 0; i < 30; i++) {
        read_gpio_values[i] = gpio_read_level(i);
        read_gpio_modes[i] = gpio_read_mode(i);
    }

    // Display the results in a table:
    printf("+--------+---+----------+---+--------+\n"
           "|  Mode  | V | GPIO Pin | V |  Mode  |\n"
           "+--------+---+----++----+---+--------+\n");

    for (i = 0; i < 29; i+=2) {
        printf("|  %s   | %d | %2d || %2d | %d |  %s   |\n",
               (read_gpio_modes[i] == GPIO_INPUT) ? "INP" : "OUT",
               read_gpio_values[i], i, i+1, read_gpio_values[i+1],
               (read_gpio_modes[i+1] == GPIO_INPUT) ? "INP" : "OUT");
    }

    printf("+--------+---+----------+---+--------+\n"
           "|  Mode  | V | GPIO Pin | V |  Mode  |\n"
           "+--------+---+----++----+---+--------+\n");
}

// Simple GPIO test script
int main(void) {
    // Store values and modes:
    int read_gpio_values[40];
    int read_gpio_modes[40];

    int i;

    // Read all pins:
    for (i = 0; i < 30; i++) {
        read_gpio_values[i] = gpio_read_level(i);
        read_gpio_modes[i] = gpio_read_mode(i);
    }

    // Begin by reading modes and levels:
    printf("Reading all pins\n");

    // This will prints results to the screen in a table:
    read_gpio();

    // Next by setting all to output:
    printf("\nAll pins to output\n");

    // Set all pins to output:
    for (i = 0; i < 30; i++) {
        gpio_set_mode(GPIO_OUTPUT, i);
    }

    read_gpio();

    // Then set (V = 1):
    printf("\nAll pins set\n");

    // Set all pins:
    for (i = 0; i < 30; i++) {
        gpio_set(i);
    }

    read_gpio();

    // Next clear (V = 0):
    printf("\nAll pins cleared\n");

    // Clear all pins:
    for (i = 0; i < 30; i++) {
        gpio_clear(i);
    }

    read_gpio();

    // Lastly, set to input:
    printf("\nAll pins to input\n");

    // Set all pins to output
    for (i = 0; i < 30; i++) {
        gpio_set_mode(GPIO_INPUT, i);
    }

    read_gpio();

    // Even more lastly, set to modes and levels back to where they were:
    printf("\nAll pins reverted\n");

    // Set all pins to output
    for (i = 0; i < 30; i++) {
        // The read modes will map directly to GPIO_INPUT and GPIO_OUTPUT:
        gpio_set_mode(read_gpio_modes[i], i);

        // Set or clear depending on last read value:
        if (read_gpio_values[i] == 1) {
            gpio_set(i);
        } else {
            gpio_clear(i);
        }
    }

    read_gpio();
}