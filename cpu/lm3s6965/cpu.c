/*
 * Copyright (C) 2017 Joseph Kroesche
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_lm3s6965
 * @{
 *
 * @file
 * @brief       Implementation of the CPU initialization
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 * @}
 */

#include "cpu.h"
#include "periph/init.h"

#include "hw_sysctl.h"
#include "periph_conf.h"    // board peripheral config
#include "periph_cpu.h"

#ifdef LM3S_USE_PLL
#define CPU_USE_PLL true
#else
#define CPU_USE_PLL false
#endif

#ifndef LM3S_XTAL
#error "You must set LM3S_XTAL to something (see periph_cpu.h)"
#endif

#ifndef LM3S_SYSDIV
#error "You must set LM3S_SYSDIV to something (see periph_cpu.h)"
#endif

#if defined(LM3S_USE_PLL) && (LM3S_XTAL == LM3S_XTAL_NONE)
#error "You must set LM3S_XTAL to a value if using LM3S_USE_PLL"
#endif

#if defined(LM3S_USE_PLL) && (LM3S_SYSDIV < LM3S_SYSDIV_4)
#error "You must set LM3S_SYSDIV to _4 or higher if LM3S_USE_PLL (see periph_cpu.h)"
#endif

static void __attribute__((naked)) cpu_delay(uint32_t count)
{
    __asm("    subs    r0, #1\n"
          "    bne     cpu_delay\n"
          "    bx      lr");
}

void lm3s_set_clock(bool use_pll, uint32_t xtal, uint32_t sysdiv)
{
    // generally follows the logiv of the driverlib function
    // SysCtlClockSet(), but without the RCC2 and with fewer options

    // cache the RCC register
    uint32_t rcc = SYSCTL->RCC;

    // bypass the PLL and disable sysdiv
    rcc &= ~SYSCTL_RCC_USE_SYSDIV;
    rcc |= SYSCTL_RCC_BYPASS;
    SYSCTL->RCC = rcc;

    // check if oscillator config needs to be changed
    if (((rcc & SYSCTL_RCC_IOSCDIS) && (LM3S_XTAL == LM3S_XTAL_NONE)) ||
        ((rcc & SYSCTL_RCC_MOSCDIS) && (LM3S_XTAL != LM3S_XTAL_NONE)))
    {
        // figure out which osc needs to be powered
        rcc &= (LM3S_XTAL == LM3S_XTAL_NONE) ? ~SYSCTL_RCC_IOSCDIS
                                             : ~SYSCTL_RCC_MOSCDIS;
        SYSCTL->RCC = rcc;
        // wait for stabilize.  this number taken from driverlib
        cpu_delay(524288);
    }
    // required oscillator should now be powered up

    // set the oscillator source (and xtal if needed)
    rcc &= ~(SYSCTL_RCC_OSCSRC_M | SYSCTL_RCC_XTAL_M);
    rcc |= (LM3S_XTAL == LM3S_XTAL_NONE) ? SYSCTL_RCC_OSCSRC_INT
                                         : SYSCTL_RCC_OSCSRC_MAIN;
    if (LM3S_XTAL != LM3S_XTAL_NONE)
    {
        rcc |= xtal;    // caller supplied xtal selection
    }
    SYSCTL->RCC = rcc;
    cpu_delay(16);      // stabilization delay from driverlib

    // now running from required oscillator with no divider
    // and xtal has been selected

    // if PLL is used then power up the PLL
    if (use_pll)
    {
        rcc &= ~SYSCTL_RCC_PWRDN;
        // clear PLL lock interrupt and write the updated RCC
        SYSCTL->MISC = SYSCTL_MISC_PLLLMIS;
        SYSCTL->RCC = rcc;

        // wait for PLL lock (with timeout)
        for (unsigned int i = 32768; i > 0; --i)
        {
            if (SYSCTL->RIS & SYSCTL_RIS_PLLLRIS)
            {
                break;
            }
        }

        // turn off PLL bypass but dont write reg yet
        rcc &= ~SYSCTL_RCC_BYPASS;
    }

    // set system divider if needed
    if (sysdiv != LM3S_SYSDIV_1)
    {
        rcc &= ~SYSCTL_RCC_SYSDIV_M;
        rcc |= sysdiv;
        rcc |= SYSCTL_RCC_USESYSDIV;
        // dont write reg yet
    }

    // turn off the unused oscillator
    rcc |= (LM3S_XTAL == LM3S_XTAL_NONE) ? SYSCTL_RCC_MOSCDIS
                                         : SYSCTL_RCC_IOSCDIS;

    // write the final value to RCC
    // this will switch to PLL (if used) and set the divider (if used)
    // and turn off unused oscillator
    SYSCTL->RCC = rcc;
    cpu_delay(16);      // stabilization delay from driverlib
}

static const uint32_t xtal_table[] =
{
    1000000U, 1843200U, 2000000U, 2457600U,
    3579545U, 3686400U, 4000000U, 4096000U,
    4915200U, 5000000U, 5120000U, 6000000U,
    6144000U, 7372800U, 8000000U, 8192000U
};

uint32_t lm3s_get_clock(void)
{
    // NOTE this function ignores the possibilty of using oscillator
    // other than main or internal
    uint32_t sysclk = 0;

    // get the RCC register which has the clock configuration
    uint32_t rcc = SYSCTL->RCC;

    // using PLL?
    if (rcc & SYSCTL_RCC_BYPASS)    // not using PLL
    {
        // figure out which OSC
        if ((rcc & SYSCTL_RCC_OSCSRC_M) == SYSCTL_RCC_OSCSRC_MAIN)
        {
            // main oscillator, now figure out xtal index
            uint32_t idx = (rcc & SYSCTL_RCC_XTAL_M) >> SYSCTL_RCC_XTAL_S;
            sysclk = xtal_table[idx];
        }
        else    // assume internal
        {
            sysclk = 12000000U;
        }
    }
    else    // using PLL
    {
        // PLL provides time base of 200 MHz (after /2)
        // It can be calculated from PLLCFG register but that is
        // a pain and it is supposed to be 200 if you are not doing
        // anything weird with the clocking
        sysclk = 200000000U;
        // assume sysdiv is used and will be divided in the next section
    }

    // apply sysdiv if used
    if (rcc & SYSCTL_RCC_USE_SYSDIV)
    {
        uint32_t sysdiv = (rcc & SYSCTL_RCC_SYSDIV_M) >> SYSCTL_RCC_SYSDIV_S;
        if (sysdiv == 0)    // special case
        {
            sysdiv = 2;
        }
        else
        {
            ++sysdiv;   // add 1 to get actual divider
        }
        sysclk /= sysdiv;
    }
    return sysclk;
}

/**
 * @brief Initialize the CPU, set IRQ priorities
 */
void cpu_init(void)
{
    /* initialize the Cortex-M core */
    cortexm_init();

    lm3s_set_clock(CPU_USE_PLL, LM3S_XTAL, LM3S_SYSDIV);

    /* trigger static peripheral initialization */
    periph_init();
}
