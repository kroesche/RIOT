/*
 * Copyright (C) 2017 Joseph Kroesche
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    boards_ek-lm3s6965 EK-LM3S6965
 * @ingroup     boards
 * @brief       Support for the Stellaris EK-LM3S6965
 * @{
 *
 * @file
 * @brief       Board specific definitions for the Stellaris EK-LM3S6965 evaluation board.
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Macros for controlling the on-board LEDs.
 * @{
 */
//#define LED0_PIN            GPIO_PIN(PORT_C, 9)
//#define LED1_PIN            GPIO_PIN(PORT_C, 8)

//#define LED_PORT            GPIOC
//#define LED0_MASK           (1 << 9)
//#define LED1_MASK           (1 << 8)

//#define LED0_ON             (LED_PORT->BSRR = LED0_MASK)
//#define LED0_OFF            (LED_PORT->BRR  = LED0_MASK)
//#define LED0_TOGGLE         (LED_PORT->ODR ^= LED0_MASK)

//#define LED1_ON             (LED_PORT->BSRR = LED1_MASK)
//#define LED1_OFF            (LED_PORT->BRR  = LED1_MASK)
//#define LED1_TOGGLE         (LED_PORT->ODR ^= LED1_MASK)
/** @} */

/**
 * @brief User button
 * @{
 */
//#define BTN0_PIN            GPIO_PIN(PORT_A, 0)
//#define BTN0_MODE           GPIO_IN
/** @} */

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
