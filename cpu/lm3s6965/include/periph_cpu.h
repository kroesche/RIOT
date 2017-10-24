/*
 * Copyright (C) 2017 Joseph Kroesche <kroesche@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_lm3s6965
 * @{
 *
 * @file
 * @brief           CPU specific definitions for internal peripheral handling
 *
 * @author          Joseph Kroesche <kroesche@gmail.com>
 */

#ifndef PERIPH_CPU_H
#define PERIPH_CPU_H

#include <stdint.h>
#include <stdbool.h>

#include "cpu_conf.h"
//#include "periph/gpio.h"
#ifdef __cplusplus
extern "C" {
#endif

// CLOCK CHOICES
//
// Internal Oscillator 12 MHz +/- 30% (no PLL)
// - does not use external xtal or osc
//     - #define LM3S_XTAL LM3S_XTAL_NONE
// - cannot be used to drive PLL
//     - do not define LM3S_USE_PLL (or #undef LM3S_USE_PLL)
//
// Main oscillator, no PLL (external xtal or osc)
// - #define LM3S_XTAL LM3S_XTAL_8MHZ (for example)
// - do not define LM3S_USE_PLL or #undef LM3S_USE_PLL
//
// Main oscillator, with PLL
// - #define LM3S_XTAL LM3S_XTAL_8MHZ (for example)
// - #define LM3S_USE_PLL
// - #define LM3S_SYSDIV LM3S_SYSDIV_4 (must be 4 or higher)
//
// System clock divider (any configuration)
// - no divider
//     - #define LM3S_SYSDIV LM3S_SYSDIV_1
// - with system clock divider
//     - #define LM3S_SYSDIV LM3S_SYSDIV_4 (for example)

// *****
// These values should be set in your board level periph_conf.h
// *****

// #define LM3S_XTAL to one of the following
#define LM3S_XTAL_NONE      (~0)
#define LM3S_XTAL_1MHZ      SYSCTL_RCC_XTAL_1MHZ
#define LM3S_XTAL_1_84MHZ   SYSCTL_RCC_XTAL_1_84MHZ
#define LM3S_XTAL_2MHZ      SYSCTL_RCC_XTAL_2MHZ
#define LM3S_XTAL_2_45MHZ   SYSCTL_RCC_XTAL_2_45MHZ
#define LM3S_XTAL_3_57MHZ   SYSCTL_RCC_XTAL_3_57MHZ
#define LM3S_XTAL_3_68MHZ   SYSCTL_RCC_XTAL_3_68MHZ
#define LM3S_XTAL_4MHZ      SYSCTL_RCC_XTAL_4MHZ
#define LM3S_XTAL_4_09MHZ   SYSCTL_RCC_XTAL_4_09MHZ
#define LM3S_XTAL_4_91MHZ   SYSCTL_RCC_XTAL_4_91MHZ
#define LM3S_XTAL_5MHZ      SYSCTL_RCC_XTAL_5MHZ
#define LM3S_XTAL_5_12MHZ   SYSCTL_RCC_XTAL_5_12MHZ
#define LM3S_XTAL_6MHZ      SYSCTL_RCC_XTAL_6MHZ
#define LM3S_XTAL_6_14MHZ   SYSCTL_RCC_XTAL_6_14MHZ
#define LM3S_XTAL_7_37MHZ   SYSCTL_RCC_XTAL_7_37MHZ
#define LM3S_XTAL_8MHZ      SYSCTL_RCC_XTAL_8MHZ
#define LM3S_XTAL_8_19MHZ   SYSCTL_RCC_XTAL_8_19MHZ

// #define LM3S_SYSDIV to one of the following
#define LM3S_SYSDIV_1       0
#define LM3S_SYSDIV_2       SYSCTL_RCC_SYSDIV_2
#define LM3S_SYSDIV_3       SYSCTL_RCC_SYSDIV_3
#define LM3S_SYSDIV_4       SYSCTL_RCC_SYSDIV_4
#define LM3S_SYSDIV_5       SYSCTL_RCC_SYSDIV_5
#define LM3S_SYSDIV_6       SYSCTL_RCC_SYSDIV_6
#define LM3S_SYSDIV_7       SYSCTL_RCC_SYSDIV_7
#define LM3S_SYSDIV_8       SYSCTL_RCC_SYSDIV_8
#define LM3S_SYSDIV_9       SYSCTL_RCC_SYSDIV_9
#define LM3S_SYSDIV_10      SYSCTL_RCC_SYSDIV_10
#define LM3S_SYSDIV_11      SYSCTL_RCC_SYSDIV_11
#define LM3S_SYSDIV_12      SYSCTL_RCC_SYSDIV_12
#define LM3S_SYSDIV_13      SYSCTL_RCC_SYSDIV_13
#define LM3S_SYSDIV_14      SYSCTL_RCC_SYSDIV_14
#define LM3S_SYSDIV_15      SYSCTL_RCC_SYSDIV_15
#define LM3S_SYSDIV_16      SYSCTL_RCC_SYSDIV_16

// If you want PLL, set the following in your periph_conf.h
// #define LM3S_USE_PLL


// CPU peripherals available
// common practice seems to be to define these in <board>/periph_conf.h
// but really these are fixed by the MCU not the board.
#define UART_NUMOF (3)
#define GPIO_NUMOF (7)

#define HAVE_GPIO_T
typedef uint32_t gpio_t;

#define HAVE_GPIO_MODE_T
#define LM3S_GPIO_MODE_OUT      0x01
#define LM3S_GPIO_MODE_IN       0x00
#define LM3S_GPIO_MODE_PU       0x02
#define LM3S_GPIO_MODE_PD       0x04
#define LM3S_GPIO_MODE_OD       0x08

typedef enum
{
    GPIO_IN     = LM3S_GPIO_MODE_IN,
    GPIO_IN_PD  = LM3S_GPIO_MODE_IN | LM3S_GPIO_MODE_PD,
    GPIO_IN_PU  = LM3S_GPIO_MODE_IN | LM3S_GPIO_MODE_PU,
    GPIO_OUT    = LM3S_GPIO_MODE_OUT,
    GPIO_OD     = LM3S_GPIO_MODE_OUT | LM3S_GPIO_MODE_OD,
    GPIO_OD_PU  = LM3S_GPIO_MODE_OUT | LM3S_GPIO_MODE_OD | LM3S_GPIO_MODE_PU
} gpio_mode_t;

typedef enum
{
    LM3S_GPIO_DRIVE_2,
    LM3S_GPIO_DRIVE_4,
    LM3S_GPIO_DRIVE_8
} gpio_drive_t;


extern void lm3s_set_clock(bool use_pll, uint32_t xtal, uint32_t sysdiv);
extern uint32_t lm3s_get_clock(void);
extern void gpio_set_alt_fn(gpio_t pin, bool is_alt);
extern void gpio_set_drive(gpio_t pin, gpio_drive_t drive);



#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CPU_H */
/** @} */
