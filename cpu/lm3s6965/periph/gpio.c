
#include <stdint.h>

#include "hw_uart.h"
#include "hw_ints.h"

#include "periph/gpio.h"

// port is specified in bits 7:4.
// 0-A, 1-B, etc
#define PORT_IDX(p) (((p) >> 4) & 0xF)
#define PORT_PIN(p) ((p) & 0xF)

static GPIOA_Type * const gpio_table[GPIO_NUMOF] =
{
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG
};
#if 0
static const uint32_t gpio_int_table[GPIO_NUMOF] =
{
    INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF, INT_GPIOG
};
#endif

int gpio_init(gpio_t pin, gpio_mode_t mode)
{
    // TODO validate inputs

    uint32_t portidx = PORT_IDX(pin);   // extract GPIO port

    // enable the GPIO clock gating in case it is not already enabled
    // do this as early as possible because it takes some cycles before
    // GPIO can be accessed after clocking is enabled
    SYSCTL->RCGC2 |= 1 << portidx;

    pin = PORT_PIN(pin);                // extract the pin
    uint32_t pin_mask = 1 << pin;

    // get GPIO instance
    GPIOA_Type * const pgpio = gpio_table[portidx];
    // data reg (DR) array to allow individual pin access
    uint32_t * const pmask = (uint32_t * const)pgpio;

    // determine if open drain
    if (mode & LM3S_GPIO_MODE_OD)
    {
        pgpio->ODR |= pin_mask;
    }
    else
    {
        pgpio->ODR &= ~pin_mask;
    }

    // set the digital enable
    pgpio->DEN |= pin_mask;

    // handle pull-up
    if (mode & LM3S_GPIO_MODE_PU)
    {
        pgpio->PUR |= pin_mask;
    }
    else
    {
        pgpio->PUR &= ~pin_mask;
    }

    // handle pull-down
    if (mode & LM3S_GPIO_MODE_PD)
    {
        pgpio->PDR |= pin_mask;
    }
    else
    {
        pgpio->PDR &= ~pin_mask;
    }

    // set direction
    if (mode & LM3S_GPIO_MODE_OUT)
    {
        // read the pin value and set it back so that once enabled
        // as an output, the state does not change (in theory)
        uint32_t pin_val = pmask[pin_mask << 2];
        pmask[pin_mask << 2] = pin_val;     // not sure if this does anything
        pgpio->DIR |= pin_mask;
    }
    else    // input
    {
        pgpio->DIR &= ~pin_mask;
    }

    return 0;
}

int gpio_init_int(gpio_t pin, gpio_mode_t mode, gpio_flank_t flank,
                  gpio_cb_t cb, void *arg)
{
    return 0;
}

void gpio_irq_enable(gpio_t pin)
{

}

void gpio_irq_disable(gpio_t pin)
{

}

int gpio_read(gpio_t pin)
{
    pin = PORT_PIN(pin);                // extract the pin
    uint32_t pin_mask = 1 << pin;

    // data reg (DR) array to allow individual pin access
    uint32_t * const pmask = (uint32_t * const)gpio_table[PORT_IDX(pin)];
    uint32_t pin_val = pmask[pin_mask << 2];
    return pin_val;
}

void gpio_set(gpio_t pin)
{
    pin = PORT_PIN(pin);                // extract the pin
    uint32_t pin_mask = 1 << pin;

    // data reg (DR) array to allow individual pin access
    uint32_t * const pmask = (uint32_t * const)gpio_table[PORT_IDX(pin)];
    pmask[pin_mask << 2] = 0xFF;
}

void gpio_clear(gpio_t pin)
{
    pin = PORT_PIN(pin);                // extract the pin
    uint32_t pin_mask = 1 << pin;

    // data reg (DR) array to allow individual pin access
    uint32_t * const pmask = (uint32_t * const)gpio_table[PORT_IDX(pin)];
    pmask[pin_mask << 2] = 0;
}

void gpio_toggle(gpio_t pin)
{
    pin = PORT_PIN(pin);                // extract the pin
    uint32_t pin_mask = 1 << pin;

    // data reg (DR) array to allow individual pin access
    uint32_t * const pmask = (uint32_t * const)gpio_table[PORT_IDX(pin)];
    pmask[pin_mask << 2] = ~pmask[pin_mask << 2];
}

void gpio_write(gpio_t pin, int value)
{
    if (value)
    {
        gpio_set(pin);
    }
    else
    {
        gpio_clear(pin);
    }
}

void gpio_set_alt_fn(gpio_t pin, bool is_alt)
{
    // get GPIO instance
    GPIOA_Type * const pgpio = gpio_table[PORT_IDX(pin)];
    uint32_t pin_mask = 1 << PORT_PIN(pin);
    pgpio->AFSEL = is_alt ? pgpio->AFSEL | pin_mask : pgpio->AFSEL & ~pin_mask;
}

void gpio_set_drive(gpio_t pin, gpio_drive_t drive)
{
    // get GPIO instance
    GPIOA_Type * const pgpio = gpio_table[PORT_IDX(pin)];
    uint32_t pin_mask = 1 << PORT_PIN(pin);
    switch (drive)
    {
        case LM3S_GPIO_DRIVE_2:
        {
            pgpio->DR2R |= pin_mask;
            break;
        }
        case LM3S_GPIO_DRIVE_4:
        {
            pgpio->DR4R |= pin_mask;
            break;
        }
        case LM3S_GPIO_DRIVE_8:
        {
            pgpio->DR8R |= pin_mask;
            break;
        }
        default:
        {
            break;
        }
    }
}
