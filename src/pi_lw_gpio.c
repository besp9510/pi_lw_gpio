// Lightweight GPIO Interface Library for the Raspberry Pi
//
// Copyright (c) 2020 Benjamin Spencer
// ============================================================================
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// ============================================================================

// Include C standard libraries:
#include <stdint.h> // C Standard integer types
#include <errno.h>  // C Standard for error conditions

// Include C POSIX libraries:
#include <sys/mman.h> // Memory management library

// Include header files:
#include "pi_lw_gpio.h"     // Macro definitions
#include "bcm.h"            // Physical address definitions
#include "map_peripheral.h" // Map peripherals into virtual memory
#include "get_pi_version.h" // Determines PI versions

// Virtual address of GPIO peripheral base:
volatile uintptr_t *gpio_base_virt_addr;

// Configured?
static int config_gpio_flag = 0;

// Configure for GPIO by mapping peripheral into virtual address space
static int config_gpio(void) {
    int pi_version;

    int bcm_peri_base_phys_addr;
    int gpio_base_phys_addr;

    // Get PI version by parsing /proc/cpu_info:
    pi_version = get_pi_version__();

    // Set BCM base addresses according to the found PI version:
    if ((pi_version == 0) || (pi_version == 1)) {
        // Set BCM base addresses:
        bcm_peri_base_phys_addr = BCM2835_PERI_BASE_PHYS_ADDR;
    } else if ((pi_version == 2) || (pi_version == 3)) {
        // Set BCM base addresses:
        bcm_peri_base_phys_addr = BCM2837_PERI_BASE_PHYS_ADDR;
    } else if (pi_version == 4) {
        // Set BCM base addresses:
        bcm_peri_base_phys_addr = BCM2711_PERI_BASE_PHYS_ADDR;
    } else {
        return -ENOPIVER;
    }

    // Add GPIO peripheral offset to find base physical address:
    gpio_base_phys_addr = (bcm_peri_base_phys_addr + 0x200000);

    // Map physical address into virtual address space to manipulate registers:
    gpio_base_virt_addr = map_peripheral__(gpio_base_phys_addr);

    if ((void *) gpio_base_phys_addr == MAP_FAILED) {
        return (int) gpio_base_virt_addr;
    }

    // Set flag so only one mapping is done:
    config_gpio_flag = 1;

    return 0;
}

// Check if GPIO pin is in range
static int check_gpio_pin(int p) {
    // There will never be more than 31 GPIO pins:
    if (p > 31) {
        return -EINVAL;
    } else {
        return 0;
    }
}

int gpio_set(int p) {
    int ret;

    // Check if GPIO has been configured and peripheral is
    // mapped into the virtual address space:
    if (!config_gpio_flag) {
        if ((ret = config_gpio()) < 0) {
            return ret;
        };
    }

    // Check if input GPIO is in range:
    if ((ret = check_gpio_pin(p)) < 0) {
        return ret;
    }

    *(gpio_base_virt_addr + 7) = (1 << p);

    return 0;
}

int gpio_clear(int p) {
    int ret;

    // Check if GPIO has been configured and peripheral is
    // mapped into the virtual address space:
    if (!config_gpio_flag) {
        if ((ret = config_gpio()) < 0) {
            return ret;
        };
    }

    // Check if input GPIO is in range:
    if ((ret = check_gpio_pin(p)) < 0) {
        return ret;
    }

    *(gpio_base_virt_addr + 10) = (1 << p);

    return 0;
}

int gpio_read_level(int p) {
    int level;
    int ret;

    // Check if GPIO has been configured and peripheral is
    // mapped into the virtual address space:
    if (!config_gpio_flag) {
        if ((ret = config_gpio()) < 0) {
            return ret;
        };
    }

    // Check if input GPIO is in range:
    if ((ret = check_gpio_pin(p)) < 0) {
        return ret;
    }

    level = (*(gpio_base_virt_addr + 13) & (1 << p));

    // If set, return 1 and not it's binary equivalent:
    if (level > 0) {
        level = 1;
    }

    return level;
}

int gpio_read_mode(int p) {
    int ret;

    // Check if GPIO has been configured and peripheral is
    // mapped into the virtual address space:
    if (!config_gpio_flag) {
        if ((ret = config_gpio()) < 0) {
            return ret;
        };
    }

    // Check if input GPIO is in range:
    if ((ret = check_gpio_pin(p)) < 0) {
        return ret;
    }

    return *(gpio_base_virt_addr + ((p)/10)) & (7 << (((p) % 10)*3));
}

int gpio_set_mode(int mode, int p) {
    int ret;

    // Check if GPIO has been configured and peripheral is
    // mapped into the virtual address space:
    if (!config_gpio_flag) {
        if ((ret = config_gpio()) < 0) {
            return ret;
        };
    }

    // Check if input GPIO is in range:
    if ((ret = check_gpio_pin(p)) < 0) {
        return ret;
    }

    // Check for correct input mode:
    if ((mode == GPIO_INPUT) || (mode == GPIO_OUTPUT)) {
        // Set to input so state can be set to output:
        *(gpio_base_virt_addr + ((p)/10)) &= ~(7 << (((p) % 10)*3));
    } else {
        return -EINVAL;
    }

    // Set mode to output if desired:
    if (mode == GPIO_OUTPUT) {
        *(gpio_base_virt_addr + ((p)/10)) |= (1 << (((p) % 10)*3));
    }

    return 0;
}
