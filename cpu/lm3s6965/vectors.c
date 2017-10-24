/*
 * Copyright (C) 2017 Joseph Kroesche <kroesche@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_lm3s6965
 * @{
 *
 * @file        vectors.c
 * @brief       Interrupt vector definitions
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 */

#include <stdint.h>
#include "vectors_cortexm.h"

/* get the start of the ISR stack as defined in the linkerscript */
extern uint32_t _estack;

/* define a local dummy handler as it needs to be in the same compilation unit
 * as the alias definition */
void dummy_handler(void) {
    dummy_handler_default();
}

/* LM3S6965 specific interrupt vectors */
WEAK_DEFAULT void isr_gpio_porta(void);
WEAK_DEFAULT void isr_gpio_portb(void);
WEAK_DEFAULT void isr_gpio_portc(void);
WEAK_DEFAULT void isr_gpio_portd(void);
WEAK_DEFAULT void isr_gpio_porte(void);
WEAK_DEFAULT void isr_uart0(void);
WEAK_DEFAULT void isr_uart1(void);
WEAK_DEFAULT void isr_ssi0(void);
WEAK_DEFAULT void isr_i2c0(void);
WEAK_DEFAULT void isr_pwm0_fault(void);
WEAK_DEFAULT void isr_pwm0_0(void);
WEAK_DEFAULT void isr_pwm0_1(void);
WEAK_DEFAULT void isr_pwm0_2(void);
WEAK_DEFAULT void isr_qei0(void);
WEAK_DEFAULT void isr_adc0_seq0(void);
WEAK_DEFAULT void isr_adc0_seq1(void);
WEAK_DEFAULT void isr_adc0_seq2(void);
WEAK_DEFAULT void isr_adc0_seq3(void);
WEAK_DEFAULT void isr_wdt(void);
WEAK_DEFAULT void isr_timer0a(void);
WEAK_DEFAULT void isr_timer0b(void);
WEAK_DEFAULT void isr_timer1a(void);
WEAK_DEFAULT void isr_timer1b(void);
WEAK_DEFAULT void isr_timer2a(void);
WEAK_DEFAULT void isr_timer2b(void);
WEAK_DEFAULT void isr_comp0(void);
WEAK_DEFAULT void isr_comp1(void);
WEAK_DEFAULT void isr_sysctl(void);
WEAK_DEFAULT void isr_flashctl(void);
WEAK_DEFAULT void isr_gpio_portf(void);
WEAK_DEFAULT void isr_gpio_portg(void);
WEAK_DEFAULT void isr_uart2(void);
WEAK_DEFAULT void isr_timer3a(void);
WEAK_DEFAULT void isr_timer3b(void);
WEAK_DEFAULT void isr_i2c1(void);
WEAK_DEFAULT void isr_qei1(void);
WEAK_DEFAULT void isr_eth(void);
WEAK_DEFAULT void isr_hibernate(void);

/* CPU specific interrupt vector table */
ISR_VECTOR(1) const isr_t vector_cpu[CPU_IRQ_NUMOF] = {
    isr_gpio_porta,        /* GPIO Port A                      16 */
    isr_gpio_portb,        /* GPIO Port B                      17 */
    isr_gpio_portc,        /* GPIO Port C                      18 */
    isr_gpio_portd,        /* GPIO Port D                      19 */
    isr_gpio_porte,        /* GPIO Port E                      20 */
    isr_uart0,             /* UART 0                           21 */
    isr_uart1,             /* UART 1                           22 */
    isr_ssi0,              /* SSI 0                            23 */
    isr_i2c0,              /* I2C 0                            24 */
    isr_pwm0_fault,        /* PWM0 Fault                       25 */
    isr_pwm0_0,            /* PWM0 Generator 0                 26 */
    isr_pwm0_1,            /* PWM0 Generator 1                 27 */
    isr_pwm0_2,            /* PWM0 Generator 2                 28 */
    isr_qei0,              /* Quadrature Encoder 0             29 */
    isr_adc0_seq0,         /* ADC 0 Seq 0                      30 */
    isr_adc0_seq1,         /* ADC 0 Seq 1                      31 */
    isr_adc0_seq2,         /* ADC 0 Seq 2                      32 */
    isr_adc0_seq3,         /* ADC 0 Seq 3                      33 */
    isr_wdt,               /* WDT 0 and 1                      34 */
    isr_timer0a,           /* 16/32 bit timer 0 A              35 */
    isr_timer0b,           /* 16/32 bit timer 0 B              36 */
    isr_timer1a,           /* 16/32 bit timer 1 A              37 */
    isr_timer1b,           /* 16/32 bit timer 1 B              38 */
    isr_timer2a,           /* 16/32 bit timer 2 A              39 */
    isr_timer2b,           /* 16/32 bit timer 2 B              40 */
    isr_comp0,             /* Analog comparator 0              41 */
    isr_comp1,             /* Analog comparator 1              42 */
    (0UL),                 /* Reserved (Analog comparator 2)   43 */
    isr_sysctl,            /* System control                   44 */
    isr_flashctl,          /* Flash + EEPROM control           45 */
    isr_gpio_portf,        /* GPIO Port F                      46 */
    isr_gpio_portg,        /* GPIO Port G                      47 */
    (0UL),                 /* Reserved (GPIO Port H)           48 */
    isr_uart2,             /* UART 2                           49 */
    (0UL),                 /* Reserved (SSI 1)                 50 */
    isr_timer3a,           /* 16/32 bit timer 3 A              51 */
    isr_timer3b,           /* 16/32 bit timer 3 B              52 */
    isr_i2c1,              /* I2C 1                            53 */
    isr_qei1,              /* Quadrature Encoder 1             54 */
    (0UL),                 /* Reserved (CAN 0)                 55 */
    (0UL),                 /* Reserved (CAN 1)                 56 */
    (0UL),                 /* Reserved (CAN 2)                 57 */
    isr_eth,               /* Ethernet                         58 */
    isr_hibernate,         /* Hibernation module               59 */
};
/** @} */
