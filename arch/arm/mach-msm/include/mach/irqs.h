/*
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2008-2014, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
>>>>>>> v3.18
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ASM_ARCH_MSM_IRQS_H
#define __ASM_ARCH_MSM_IRQS_H

<<<<<<< HEAD
/*
 * 0-15:  STI/SGI (software triggered/generated interrupts)
 * 16-31: PPI (private peripheral interrupts)
 * 32+:   SPI (shared peripheral interrupts)
 */
#define GIC_PPI_START 16
#define GIC_SPI_START 32

#define MSM_IRQ_BIT(irq)     (1 << ((irq) & 31))

#if !defined(CONFIG_SPARSE_IRQ)

#if defined(CONFIG_ARCH_MSM8974)
#define NR_MSM_IRQS 1020 /* Should be 256 - but higher due to bug in sim */
#define NR_GPIO_IRQS 146
#define NR_QPNP_IRQS 32768
#define NR_BOARD_IRQS NR_QPNP_IRQS

#elif defined(CONFIG_ARCH_MSM8610) || defined(CONFIG_ARCH_MSM8226)
#define NR_MSM_IRQS 256
#define NR_GPIO_IRQS 117
#define NR_QPNP_IRQS 32768
#define NR_BOARD_IRQS NR_QPNP_IRQS

=======
#define MSM_IRQ_BIT(irq)     (1 << ((irq) & 31))

#if defined(CONFIG_ARCH_MSM7X30)
#include "irqs-7x30.h"
#elif defined(CONFIG_ARCH_QSD8X50)
#include "irqs-8x50.h"
#include "sirc.h"
#elif defined(CONFIG_ARCH_MSM_ARM11)
#include "irqs-7x00.h"
#else
#error "Unknown architecture specification"
>>>>>>> v3.18
#endif

#define NR_IRQS (NR_MSM_IRQS + NR_GPIO_IRQS + NR_BOARD_IRQS)
#define MSM_GPIO_TO_INT(n) (NR_MSM_IRQS + (n))
<<<<<<< HEAD
#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)
#define MSM_INT_TO_REG(base, irq) (base + irq / 32)

#endif

#if defined(CONFIG_PCI_MSI) && defined(CONFIG_MSM_PCIE)
#define MSM_PCIE_MSI_INT(n) (NR_MSM_IRQS + NR_GPIO_IRQS + NR_PM8921_IRQS +  \
		NR_PM8821_IRQS + NR_TABLA_IRQS + NR_GPIO_EXPANDER_IRQS + (n))
#endif

#endif
=======
#define MSM_INT_TO_REG(base, irq) (base + irq / 32)

#endif
>>>>>>> v3.18
