/*
 * Copyright (C) 2017 Joseph Kroesche
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_ek-lm3s6965
 * @{
 *
 * @file
 * @brief       Peripheral MCU configuration for the EK-LM3S6965 board
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include "periph_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

// this board has an 8MHz crystal
#define LM3S_XTAL LM3S_XTAL_8MHZ

// set the board default to just run from main oscillator using
// external xtal, no PLL and no system divider
// #define LM3S_USE_PLL
#define LM3S_SYSDIV LM3S_SYSDIV_1

// In this configuration MCU will be running at 8 MHz.
// Application can change clock after init if needed.

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
