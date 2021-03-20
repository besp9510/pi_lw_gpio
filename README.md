# Lightweight GPIO Interface Library for the Raspberry Pi
pi_lw_gpio.c provides a lightweight General Purpose Input Output (GPIO) interface library for the Raspberry Pi via virtual memory mapping of the GPIO peripheral. This software requires no dependencies other than Raspbian running on any version of the Raspberry Pi. Note, not all possible GPIO functions are included in this library at the present.

This project is not meant to replace the many, and much better, existing PI GPIO libraries such as [pigpio](http://abyz.me.uk/rpi/pigpio/) and [Wiring Pi](http://wiringpi.com/). The purpose is to publish the tools I created to learn more about the Pi as well as share the learning process to others. As such, this emphasizes documenting how GPIO interface via virtual memory mapping is achieved to allow anyone a better understanding of the Raspberry PI and low-level programming in general.

![gpio](images/gpio.jpg)

pi_lw_gpio.c is provided two ways for flexibility:
1. C source and header files that can be compiled along with your program
2. C shared library

To learn and understand the source code, see [raspberry_pi_gpio_programming.pdf](doc/raspberry_pi_gpio_programming.pdf) for a complete breakdown on how this GPIO interface library works.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Installing

#### Obtain the Project

First, clone this repository.

```
$ git clone https://github.com/besp9510/pi_lw_gpio.git
```

Alternatively, download the repository from Git.

#### Configure

Once obtaining a copy on your local machine, navigate to the top directory and run the configure script to generate a Makefile.

```
$ ./configure
```

By default, files will be installed under `/usr/local/`. Note that passing the option ``--help`` will display available configuration options such as installation directory prefix and debug symbols.

#### Make

Compile pi_lw_gpio.c into a shared library.

```
$ make
```

Then install files to the installation directory. You must run the following either as root or with root privileges.

```
$ sudo make install
```

#### Uninstall
At anytime, to uninstall pi_lw_gpio.c, use the same Makefile used for compiling or a Makefile generated using the configuration script with the same options as root or with root privileges.

```
$ sudo make uninstall
```

## Running the Test

pi_lw_gpio_test.c is a test script to check and see the GPIO interface library working on your PI. The outline of this test script:
1. Read all pin's mode and level
2. All pins to output
3. Set all pins
4. Clear all pins
5. All pins to input
6. Revet all pin's mode and level

To compile the test script, first navigate to the test directory `test/`. Next, run the configure script to generate the Makefile:

```
$ ./configure
```

By default, the pi_lw_gpio.c shared library will be looked for under the standard directories (e.g. `/usr/local/`). If this is not the case, pass the option ``--help`` to learn how to specify a directory to be searched. Additionally, ``--help`` will also display available configuration options such as debug symbols and debug logs.

Next, compile the test script:

```
$ make
```

This will create an executable called `pi_gpio_test` under `bin/`.

## Documentation

### How it Works
To learn and understand the source code, see [raspberry_pi_gpio_programming.pdf](doc/raspberry_pi_gpio_programming.pdf) for a complete breakdown on how this GPIO interface library works.This reference documents
1. What GPIOs are
2. BCM2837 ARM & Working with Peripherals
3. Memory Mapping
4. GPIO Function Macros

Note that this reference has register addresses specific to the BCM2836/BCM2837 (Raspberry PI 2 & 3) processor. The peripheral base physical address for the other PI versions are:
* Raspberry PI 1 & Zero (BCM2835) : 0x20000000
* Raspberry PI 4 (BCM2711) : 0xFE000000

### Functions

#### GPIO Set & Clear
Set and clear a GPIO pin. Setting a pin causes it to go high while clearing causes it to go low.

```c
int gpio_set(int p);
```

```c
int gpio_clear(int p);
```

##### Return Value
`gpio_set()` and `gpio_clear()` returns 0 upon success. On error, an error number is returned.

Error numbers:
* `ENOPIVER` : Could not get Pi board revision.
* `MAP_FAILED` : Memory map failed (most likely due to permissions)
* `EINVAL` : Invalid GPIO pin (must be in range 0 to 30)

#### GPIO Read Level
Read the level of a GPIO pin.

```c
int gpio_read_level(int p);
```

##### Return Value
`gpio_read_level()` returns `0` if pin is low or `1` if pin is high. On error, an error number is returned.

Error numbers:
* `ENOPIVER` : Could not get Pi board revision.
* `MAP_FAILED` : Memory map failed (most likely due to permissions)
* `EINVAL` : Invalid GPIO pin (must be in range 0 to 30)

#### GPIO Set Mode
Set the mode a GPIO pin. Supported modes are:
* `GPIO_INPUT`
* `GPIO_OUTPUT`

```c
int gpio_set_mode(int mode, int p);
```

##### Return Value
`gpio_set_mode()` returns 0 upon success. On error, an error number is returned.

Error numbers:
* `ENOPIVER` : Could not get Pi board revision.
* `MAP_FAILED` : Memory map failed (most likely due to permissions)
* `EINVAL` : Invalid GPIO pin (must be in range 0 to 30)

#### GPIO Read Mode
Read the current mode of a GPIO pin. Supported modes are:
* `GPIO_INPUT`
* `GPIO_OUTPUT`

```c
int gpio_read_mode(int p);
```

##### Return Value
`gpio_read_mode()` returns the following upon success.
* `GPIO_INPUT`
* `GPIO_OUTPUT`

On error, an error number is returned.

Error numbers:
* `ENOPIVER` : Could not get Pi board revision.
* `MAP_FAILED` : Memory map failed (most likely due to permissions)
* `EINVAL` : Invalid GPIO pin (must be in range 0 to 30)

## Contributing
Follow the "fork-and-pull" Git workflow.
1. Fork the repo on GitHub
2. Clone the project to your own machine
3. Commit changes to your own branch
4. Push your work back up to your fork
5. Submit a Pull request so that your changes can be reviewed

Be sure to merge the latest from "upstream" before making a pull request!

Feel free to email at the email address under my account name if you have any questions.
## Authors

Benjamin Spencer

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
