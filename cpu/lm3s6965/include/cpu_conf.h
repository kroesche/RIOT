/*
 * Copyright (C) 2017 Joseph Kroesche <kroesche@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup        cpu_lm3s6965 LMI/TI LM3S6965
 * @ingroup         cpu
 * @brief           CPU specific implementations for the LMI/TI LM3S6965
 * @{
 *
 * @file
 * @brief           CPU specific configuration options
 *
 * @author          Joseph Kroesche <kroesche@gmail.com>
 */

#ifndef CPU_CONF_H
#define CPU_CONF_H

#include "cpu_conf_common.h"

//#include "vendor/LPC17xx.h"

#include "vendor/LM3S6965.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   ARM Cortex-M specific CPU configuration
 * @{
 */
#define CPU_DEFAULT_IRQ_PRIO            (1U)
#define CPU_IRQ_NUMOF                   (44U)
#define CPU_FLASH_BASE                  FLASH_BASE
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CPU_CONF_H */
/** @} */
