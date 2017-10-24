/*
 * Copyright (C) 2017 Joseph Kroesche
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_lm3s6965
 * @{
 *
 * @brief       Linker description file for LM3S6965 microcontrollers.
 *
 * @author      Joseph Kroesche <kroesche@gmail.com>
 *
 * @}
 */
MEMORY
{
    rom (rx)    : ORIGIN = 0x00000000, LENGTH = 256K
    ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 64K
}

INCLUDE cortexm_base.ld
