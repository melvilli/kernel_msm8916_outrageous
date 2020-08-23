/* arch/arm/mach-msm/io.c
 *
 * MSM7K, QSD io support
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2008-2014, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
>>>>>>> v3.18
=======
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
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

#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/bug.h>
>>>>>>> v3.18
=======
#include <linux/bug.h>
>>>>>>> v3.18
#include <linux/init.h>
#include <linux/io.h>
#include <linux/export.h>

#include <mach/hardware.h>
#include <asm/page.h>
#include <mach/msm_iomap.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/memory.h>
#include <asm/mach/map.h>
#include <linux/dma-mapping.h>
#include <linux/of_fdt.h>

#include <mach/board.h>
#include "board-dt.h"
=======
#include <asm/mach/map.h>

#include "common.h"
>>>>>>> v3.18
=======
#include <asm/mach/map.h>

#include "common.h"
>>>>>>> v3.18

#define MSM_CHIP_DEVICE_TYPE(name, chip, mem_type) {			      \
		.virtual = (unsigned long) MSM_##name##_BASE, \
		.pfn = __phys_to_pfn(chip##_##name##_PHYS), \
		.length = chip##_##name##_SIZE, \
<<<<<<< HEAD
<<<<<<< HEAD
		.type = MT_DEVICE, \
=======
		.type = mem_type, \
>>>>>>> v3.18
=======
		.type = mem_type, \
>>>>>>> v3.18
	 }

#define MSM_DEVICE_TYPE(name, mem_type) \
		MSM_CHIP_DEVICE_TYPE(name, MSM, mem_type)
#define MSM_CHIP_DEVICE(name, chip) \
		MSM_CHIP_DEVICE_TYPE(name, chip, MT_DEVICE)
#define MSM_DEVICE(name) MSM_CHIP_DEVICE(name, MSM)

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_ARCH_MSM8974
static struct map_desc msm_8974_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(TLMM, MSM8974),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, MSM8974),
#ifdef CONFIG_DEBUG_MSM8974_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_8974_io(void)
{
	iotable_init(msm_8974_io_desc, ARRAY_SIZE(msm_8974_io_desc));
}
#endif /* CONFIG_ARCH_MSM8974 */

#ifdef CONFIG_ARCH_APQ8084
static struct map_desc msm_8084_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(MPM2_PSHOLD, APQ8084),
	MSM_CHIP_DEVICE(TLMM, APQ8084),
#ifdef CONFIG_DEBUG_APQ8084_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_8084_io(void)
{
	iotable_init(msm_8084_io_desc, ARRAY_SIZE(msm_8084_io_desc));
}
#endif /* CONFIG_ARCH_APQ8084 */

#ifdef CONFIG_ARCH_FSM9900
static struct map_desc fsm9900_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(TLMM, FSM9900),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, FSM9900),
#ifdef CONFIG_DEBUG_FSM9900_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_fsm9900_io(void)
{
	iotable_init(fsm9900_io_desc, ARRAY_SIZE(fsm9900_io_desc));
}
#endif /* CONFIG_ARCH_FSM9900 */

#ifdef CONFIG_ARCH_FSM9010
static struct map_desc fsm9010_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(APCS_GCC, FSM9010),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, FSM9010),
#ifdef CONFIG_DEBUG_FSM9010_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_fsm9010_io(void)
{
	iotable_init(fsm9010_io_desc, ARRAY_SIZE(fsm9010_io_desc));
}
#endif /* CONFIG_ARCH_FSM9010 */


#ifdef CONFIG_ARCH_MDM9630
static struct map_desc mdm9630_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(TLMM, MDM9630),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, MDM9630),
};

void __init msm_map_mdm9630_io(void)
{
	iotable_init(mdm9630_io_desc, ARRAY_SIZE(mdm9630_io_desc));
}
#endif /* CONFIG_ARCH_MDM9630 */

#if defined(CONFIG_ARCH_MSM8909) || defined(CONFIG_ARCH_MDMFERRUM)
static struct map_desc msm8909_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(APCS_GCC, MSM8909),
#if defined(CONFIG_DEBUG_MSM8909_UART) || defined(CONFIG_DEBUG_MDMFERRUM_UART)
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_msm8909_io(void)
{
	iotable_init(msm8909_io_desc, ARRAY_SIZE(msm8909_io_desc));
}
#endif /* CONFIG_ARCH_MSM8909 || CONFIG_ARCH_MDMFERRUM */

#ifdef CONFIG_ARCH_MSM8916
static struct map_desc msm8916_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(APCS_GCC, MSM8916),
#ifdef CONFIG_DEBUG_MSM8916_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_msm8916_io(void)
{
	iotable_init(msm8916_io_desc, ARRAY_SIZE(msm8916_io_desc));
}
#endif /* CONFIG_ARCH_MSM8916 */

#ifdef CONFIG_ARCH_MSM8226
static struct map_desc msm_8226_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(APCS_GCC, MSM8226),
	MSM_CHIP_DEVICE(TLMM, MSM8226),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, MSM8226),
#ifdef CONFIG_DEBUG_MSM8226_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};


void __init msm_map_msm8226_io(void)
{
	iotable_init(msm_8226_io_desc, ARRAY_SIZE(msm_8226_io_desc));
}
#endif /* CONFIG_ARCH_MSM8226 */

#ifdef CONFIG_ARCH_MSM8610
static struct map_desc msm8610_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(APCS_GCC, MSM8610),
	MSM_CHIP_DEVICE(TLMM, MSM8610),
	MSM_CHIP_DEVICE(MPM2_PSHOLD, MSM8610),
};

void __init msm_map_msm8610_io(void)
{
	iotable_init(msm8610_io_desc, ARRAY_SIZE(msm8610_io_desc));
}
#endif /* CONFIG_ARCH_MSM8610 */

#ifdef CONFIG_ARCH_MSMZIRC
static struct map_desc msmzirc_io_desc[] __initdata = {
#ifdef CONFIG_DEBUG_MSMZIRC_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_msmzirc_io(void)
{
	iotable_init(msmzirc_io_desc, ARRAY_SIZE(msmzirc_io_desc));
}
#endif /* CONFIG_ARCH_MSMZIRC */
=======
=======
>>>>>>> v3.18
#if defined(CONFIG_ARCH_MSM7X00A)
static struct map_desc msm_io_desc[] __initdata = {
	MSM_DEVICE_TYPE(VIC, MT_DEVICE_NONSHARED),
	MSM_CHIP_DEVICE_TYPE(CSR, MSM7X00, MT_DEVICE_NONSHARED),
	MSM_DEVICE_TYPE(DMOV, MT_DEVICE_NONSHARED),
	MSM_CHIP_DEVICE_TYPE(GPIO1, MSM7X00, MT_DEVICE_NONSHARED),
	MSM_CHIP_DEVICE_TYPE(GPIO2, MSM7X00, MT_DEVICE_NONSHARED),
	MSM_DEVICE_TYPE(CLK_CTL, MT_DEVICE_NONSHARED),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.pfn = __phys_to_pfn(MSM_SHARED_RAM_PHYS),
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
#if defined(CONFIG_DEBUG_MSM_UART)
	{
		/* Must be last: virtual and pfn filled in by debug_ll_addr() */
		.length = SZ_4K,
		.type = MT_DEVICE_NONSHARED,
	}
#endif
};

void __init msm_map_common_io(void)
{
	size_t size = ARRAY_SIZE(msm_io_desc);

	/* Make sure the peripheral register window is closed, since
	 * we will use PTE flags (TEX[1]=1,B=0,C=1) to determine which
	 * pages are peripheral interface or not.
	 */
	asm("mcr p15, 0, %0, c15, c2, 4" : : "r" (0));
#if defined(CONFIG_DEBUG_MSM_UART)
#ifdef CONFIG_MMU
	debug_ll_addr(&msm_io_desc[size - 1].pfn,
		      &msm_io_desc[size - 1].virtual);
#endif
	msm_io_desc[size - 1].pfn = __phys_to_pfn(msm_io_desc[size - 1].pfn);
#endif
	iotable_init(msm_io_desc, size);
}
#endif

#ifdef CONFIG_ARCH_QSD8X50
static struct map_desc qsd8x50_io_desc[] __initdata = {
	MSM_DEVICE(VIC),
	MSM_CHIP_DEVICE(CSR, QSD8X50),
	MSM_DEVICE(DMOV),
	MSM_CHIP_DEVICE(GPIO1, QSD8X50),
	MSM_CHIP_DEVICE(GPIO2, QSD8X50),
	MSM_DEVICE(CLK_CTL),
	MSM_DEVICE(SIRC),
	MSM_DEVICE(SCPLL),
	MSM_DEVICE(AD5),
	MSM_DEVICE(MDC),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.pfn = __phys_to_pfn(MSM_SHARED_RAM_PHYS),
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_qsd8x50_io(void)
{
	debug_ll_io_init();
	iotable_init(qsd8x50_io_desc, ARRAY_SIZE(qsd8x50_io_desc));
}
#endif /* CONFIG_ARCH_QSD8X50 */

#ifdef CONFIG_ARCH_MSM7X30
static struct map_desc msm7x30_io_desc[] __initdata = {
	MSM_DEVICE(VIC),
	MSM_CHIP_DEVICE(CSR, MSM7X30),
	MSM_DEVICE(DMOV),
	MSM_CHIP_DEVICE(GPIO1, MSM7X30),
	MSM_CHIP_DEVICE(GPIO2, MSM7X30),
	MSM_DEVICE(CLK_CTL),
	MSM_DEVICE(CLK_CTL_SH2),
	MSM_DEVICE(AD5),
	MSM_DEVICE(MDC),
	MSM_DEVICE(ACC),
	MSM_DEVICE(SAW),
	MSM_DEVICE(GCC),
	MSM_DEVICE(TCSR),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.pfn = __phys_to_pfn(MSM_SHARED_RAM_PHYS),
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_msm7x30_io(void)
{
	debug_ll_io_init();
	iotable_init(msm7x30_io_desc, ARRAY_SIZE(msm7x30_io_desc));
}
#endif /* CONFIG_ARCH_MSM7X30 */

#ifdef CONFIG_ARCH_MSM7X00A
void __iomem *__msm_ioremap_caller(phys_addr_t phys_addr, size_t size,
				   unsigned int mtype, void *caller)
{
	if (mtype == MT_DEVICE) {
		/* The peripherals in the 88000000 - D0000000 range
		 * are only accessible by type MT_DEVICE_NONSHARED.
		 * Adjust mtype as necessary to make this "just work."
		 */
		if ((phys_addr >= 0x88000000) && (phys_addr < 0xD0000000))
			mtype = MT_DEVICE_NONSHARED;
	}

	return __arm_ioremap_caller(phys_addr, size, mtype, caller);
}
#endif
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
