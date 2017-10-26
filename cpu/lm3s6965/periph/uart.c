/*
 * Copyright (C) 2017 Joseph Kroesche
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_lm3s6965
 * @ingroup     drivers_periph_uart
 * @{
 *
 * @file        uart.c
 * @brief       Implementation of the low-level UART driver for the LM3S6965
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 *
 * @}
 */

#include <stdint.h>

#include "ringbuffer.h"
#include "hw_uart.h"
#include "hw_ints.h"
#include "cpu.h"
#include "periph/uart.h" // standard peripheral header
#include "periph/gpio.h"

#define uart_write_int uart_write
//#define uart_write_polling uart_write

static UART0_Type * const uart_table[UART_NUMOF] =
{
    UART0, UART1, UART2
};
static const uint32_t uart_int_table[UART_NUMOF] =
{
    INT_UART0 - 16, INT_UART1 - 16, INT_UART2 - 16
};
// GPIO pin (port/pin) of first pin (rx) of rx/tx pair.
// The other (tx) will be the same +1
static const uint32_t uart_pins[UART_NUMOF] =
{
    0x00, 0x32, 0x60
};

// TX buffers
static char txbuf[UART_NUMOF][64];
// ringbuffer structs
static ringbuffer_t ringbuf[UART_NUMOF] =
{
    RINGBUFFER_INIT(txbuf[0]),
    RINGBUFFER_INIT(txbuf[1]),
    RINGBUFFER_INIT(txbuf[2])
};

static uart_isr_ctx_t ctx[UART_NUMOF] =
{
    { NULL, NULL }, { NULL, NULL }, { NULL, NULL }
};

int uart_init(uart_t uart, uint32_t baudrate, uart_rx_cb_t rx_cb, void *arg)
{
    // TODO add line ctrl settings

    // validate UART device
    if (uart >= UART_NUMOF)
    {
        return UART_NODEV;
    }

    // enable the UART clock gating in case it is not already enabled
    SYSCTL->RCGC1 |= 1 << uart;

    // set up the UART pins
    gpio_init(uart_pins[uart], GPIO_IN);    // rx
    gpio_set_alt_fn(uart_pins[uart], true);
    gpio_init(uart_pins[uart] + 1, GPIO_OUT); // tx
    gpio_set_alt_fn(uart_pins[uart] + 1, true);

    // get UART instance
    UART0_Type * const puart = uart_table[uart];

    // shut off interrupt just in case
    NVIC_DisableIRQ(uart_int_table[uart]);

    // get the system clock
    uint32_t sysclk = lm3s_get_clock();

    // init steps from data sheet and driverlib
    puart->CTL &= ~UART_CTL_UARTEN;     // disable

    // from data sheet
    // BRD = sysclk / (16 * baudrate)
    // convert to 16.6
    // BRD = sysclk * 64 / (16 * baudrate)
    // BRD = sysclk * 4 / baudrate
    // BRD = (((sysclk * 8) / baudrate) + 1) / 2 ; rounding
    uint32_t bdiv = (((sysclk * 8) / baudrate) + 1) / 2;

    // load computed baud rate divisor into registers
    puart->IBRD = bdiv / 64;
    puart->FBRD = bdiv % 64;

    // RIOT says to always use n,8,1
    // future enhancement, add line control params
    puart->LCRH = UART_LCRH_WLEN_8 | UART_LCRH_FEN;

    // set FIFO trigger levels to half
    puart->IFLS = UART_IFLS_RX4_8 | UART_IFLS_TX4_8;

    // clear all interrupts, then enable
    puart->ICR = ~0;
    puart->IM = UART_IM_RTIM | UART_IM_TXIM | UART_IM_RXIM;

    // save the interrupt callback details
    ctx[uart].rx_cb = rx_cb;
    ctx[uart].arg = arg;

    // finally, enable interrupt on the NVIC and enable UART
    NVIC_ClearPendingIRQ(uart_int_table[uart]);
    NVIC_EnableIRQ(uart_int_table[uart]);
    puart->CTL |= UART_CTL_UARTEN;

    return UART_OK;
}

// This function is used to force one byte to be copied to TX FIFO.
// This is needed to get TX interrupts running.  If the TX FIFO is
// empty, then there is nothing to trigger a TX interrupt that will allow
// the ISR to start draining the ring buffer.  Therefore, when data is
// first written to the ring buffer, it must also write a byte to the
// TX FIFO to get the whole thing running.
static void prime_uart(uart_t uart)
{
    // get UART instance
    UART0_Type * const puart = uart_table[uart];
    NVIC_DisableIRQ(uart_int_table[uart]);

    ringbuffer_t *pring = &ringbuf[uart];

    // if there is anything in the ring buffer, try to write a byte out
    if (!ringbuffer_empty(pring))
    {
        while ((puart->FR & UART_FR_TXFF) == 0) // TX FIFO not full
        {
            int b = ringbuffer_get_one(pring);
            if (b >= 0)
            {
                puart->DR = b;
            }
            else
            {
                break;
            }
        }
    }

    NVIC_EnableIRQ(uart_int_table[uart]);
}

// This is non-blocking as long as there is room in the ring buffer but
// as soon as the ring buffer is full, then it is blocking until some of
// the data drains out via ISR
void uart_write_int(uart_t uart, const uint8_t *data, size_t len)
{
    // validate UART device
    if (uart >= UART_NUMOF)
    {
        return;
    }

    // get UART instance
    UART0_Type * const puart = uart_table[uart];
    ringbuffer_t *pring = &ringbuf[uart];

    // stay in loop until all bytes are sent
    while (len--)
    {
        // make sure tx interrupts can run
        puart->IM |= UART_IM_TXIM;
        // spin until there is space in the buffer
        while (ringbuffer_full(pring))
        {
        }
        // TX interrupt critical section to protect ring buffer
        puart->IM &= ~UART_IM_TXIM;
        ringbuffer_add_one(pring, *data);
        puart->IM |= UART_IM_TXIM;
        prime_uart(uart);
        ++data;
    }
}

void uart_write_polling(uart_t uart, const uint8_t *data, size_t len)
{
    // validate UART device
    if (uart >= UART_NUMOF)
    {
        return;
    }

    // get UART instance
    UART0_Type * const puart = uart_table[uart];

    while (len--)
    {
        while (puart->FR & UART_FR_TXFF)
        {}

        puart->DR = *data;
        ++data;
    }
}

void isr_uart0(void)
{
    // get interrupt cause flags and clear all pending
    uint32_t intflags = UART0->MIS;
    UART0->ICR = intflags;

    // receive interrupt
    if (intflags & (UART_MIS_RXMIS | UART_MIS_RTMIS))
    {
        // while there is data in RX FIFO
        while ((UART0->FR & UART_FR_RXFE) == 0)
        {
            uint8_t b = UART0->DR;  // get the byte
            // if there is a callback available then send it
            if (ctx[0].rx_cb)
            {
                ctx[0].rx_cb(ctx[0].arg, b);
            }
        }
    }

    // process transmit interrupt
    if (intflags & UART_MIS_TXMIS)
    {
        ringbuffer_t *pring = &ringbuf[0];

        // copy bytes to output FIFO while there is room
        if (!ringbuffer_empty(pring))
        {
            while ((UART0->FR & UART_FR_TXFF) == 0)
            {
                int b = ringbuffer_get_one(pring);
                if (b < 0)
                {
                    // tx buffer is empty so turn off tx interrupt
                    UART0->IM &= ~UART_IM_TXIM;
                    break;
                }
                else
                {
                    UART0->DR = b;
                }
            }
        }
        else    // nothing to send
        {
            UART0->IM &= ~UART_IM_TXIM;
        }
    }
    cortexm_isr_end();
}
