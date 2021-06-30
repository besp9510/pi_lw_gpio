// GPIO mode aliases:
#define GPIO_INPUT 0x00
#define GPIO_OUTPUT 0x01

// Error numbers:
#define ENOPIVER 2 // Could not get PI board revision

// GPIO operation function prototypes:
int gpio_set(int p);
int gpio_clear(int p);
int gpio_read_level(int p);
int gpio_set_mode(int mode, int p);
int gpio_read_mode(int p);