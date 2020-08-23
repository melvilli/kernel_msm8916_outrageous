<<<<<<< HEAD
<<<<<<< HEAD
/* linux/arch/arm/mach-exynos4/platsmp.c
 *
=======
 /*
>>>>>>> v3.18
=======
 /*
>>>>>>> v3.18
 * Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Cloned from linux/arch/arm/mach-vexpress/platsmp.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/smp.h>
#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of_address.h>
>>>>>>> v3.18
=======
#include <linux/of_address.h>
>>>>>>> v3.18

#include <asm/cacheflush.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
#include <asm/firmware.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/hardware.h>
#include <mach/regs-clock.h>
#include <mach/regs-pmu.h>

#include <plat/cpu.h>

#include "common.h"

extern void exynos4_secondary_startup(void);

static inline void __iomem *cpu_boot_reg_base(void)
{
	if (soc_is_exynos4210() && samsung_rev() == EXYNOS4210_REV_1_1)
		return S5P_INFORM5;
	return S5P_VA_SYSRAM;
=======
=======
>>>>>>> v3.18
#include <mach/map.h>

#include "common.h"
#include "regs-pmu.h"

extern void exynos4_secondary_startup(void);

/**
 * exynos_core_power_down : power down the specified cpu
 * @cpu : the cpu to power down
 *
 * Power down the specified cpu. The sequence must be finished by a
 * call to cpu_do_idle()
 *
 */
void exynos_cpu_power_down(int cpu)
{
	pmu_raw_writel(0, EXYNOS_ARM_CORE_CONFIGURATION(cpu));
}

/**
 * exynos_cpu_power_up : power up the specified cpu
 * @cpu : the cpu to power up
 *
 * Power up the specified cpu
 */
void exynos_cpu_power_up(int cpu)
{
	pmu_raw_writel(S5P_CORE_LOCAL_PWR_EN,
			EXYNOS_ARM_CORE_CONFIGURATION(cpu));
}

/**
 * exynos_cpu_power_state : returns the power state of the cpu
 * @cpu : the cpu to retrieve the power state from
 *
 */
int exynos_cpu_power_state(int cpu)
{
	return (pmu_raw_readl(EXYNOS_ARM_CORE_STATUS(cpu)) &
			S5P_CORE_LOCAL_PWR_EN);
}

/**
 * exynos_cluster_power_down : power down the specified cluster
 * @cluster : the cluster to power down
 */
void exynos_cluster_power_down(int cluster)
{
	pmu_raw_writel(0, EXYNOS_COMMON_CONFIGURATION(cluster));
}

/**
 * exynos_cluster_power_up : power up the specified cluster
 * @cluster : the cluster to power up
 */
void exynos_cluster_power_up(int cluster)
{
	pmu_raw_writel(S5P_CORE_LOCAL_PWR_EN,
			EXYNOS_COMMON_CONFIGURATION(cluster));
}

/**
 * exynos_cluster_power_state : returns the power state of the cluster
 * @cluster : the cluster to retrieve the power state from
 *
 */
int exynos_cluster_power_state(int cluster)
{
	return (pmu_raw_readl(EXYNOS_COMMON_STATUS(cluster)) &
		S5P_CORE_LOCAL_PWR_EN);
}

static inline void __iomem *cpu_boot_reg_base(void)
{
	if (soc_is_exynos4210() && samsung_rev() == EXYNOS4210_REV_1_1)
		return pmu_base_addr + S5P_INFORM5;
	return sysram_base_addr;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void __iomem *cpu_boot_reg(int cpu)
{
	void __iomem *boot_reg;

	boot_reg = cpu_boot_reg_base();
<<<<<<< HEAD
<<<<<<< HEAD
	if (soc_is_exynos4412())
		boot_reg += 4*cpu;
=======
=======
>>>>>>> v3.18
	if (!boot_reg)
		return ERR_PTR(-ENODEV);
	if (soc_is_exynos4412())
		boot_reg += 4*cpu;
	else if (soc_is_exynos5420() || soc_is_exynos5800())
		boot_reg += 4;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return boot_reg;
}

/*
 * Write pen_release in a way that is guaranteed to be visible to all
 * observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
static void write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
<<<<<<< HEAD
<<<<<<< HEAD
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));
=======
	sync_cache_w(&pen_release);
>>>>>>> v3.18
=======
	sync_cache_w(&pen_release);
>>>>>>> v3.18
}

static void __iomem *scu_base_addr(void)
{
	return (void __iomem *)(S5P_VA_SCU);
}

static DEFINE_SPINLOCK(boot_lock);

<<<<<<< HEAD
<<<<<<< HEAD
static void __cpuinit exynos_secondary_init(unsigned int cpu)
=======
static void exynos_secondary_init(unsigned int cpu)
>>>>>>> v3.18
=======
static void exynos_secondary_init(unsigned int cpu)
>>>>>>> v3.18
{
	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	write_pen_release(-1);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __cpuinit exynos_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
	unsigned long phys_cpu = cpu_logical_map(cpu);
=======
=======
>>>>>>> v3.18
static int exynos_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
	u32 mpidr = cpu_logical_map(cpu);
	u32 core_id = MPIDR_AFFINITY_LEVEL(mpidr, 0);
	int ret = -ENOSYS;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	write_pen_release(phys_cpu);

	if (!(__raw_readl(S5P_ARM_CORE1_STATUS) & S5P_CORE_LOCAL_PWR_EN)) {
		__raw_writel(S5P_CORE_LOCAL_PWR_EN,
			     S5P_ARM_CORE1_CONFIGURATION);

		timeout = 10;

		/* wait max 10 ms until cpu1 is on */
		while ((__raw_readl(S5P_ARM_CORE1_STATUS)
			& S5P_CORE_LOCAL_PWR_EN) != S5P_CORE_LOCAL_PWR_EN) {
=======
=======
>>>>>>> v3.18
	 * Note that "pen_release" is the hardware CPU core ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	write_pen_release(core_id);

	if (!exynos_cpu_power_state(core_id)) {
		exynos_cpu_power_up(core_id);
		timeout = 10;

		/* wait max 10 ms until cpu1 is on */
		while (exynos_cpu_power_state(core_id)
		       != S5P_CORE_LOCAL_PWR_EN) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			if (timeout-- == 0)
				break;

			mdelay(1);
		}

		if (timeout == 0) {
			printk(KERN_ERR "cpu1 power enable failed");
			spin_unlock(&boot_lock);
			return -ETIMEDOUT;
		}
	}
	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		unsigned long boot_addr;

		smp_rmb();

		boot_addr = virt_to_phys(exynos4_secondary_startup);

		/*
		 * Try to set boot address using firmware first
		 * and fall back to boot register if it fails.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (call_firmware_op(set_cpu_boot_addr, phys_cpu, boot_addr))
			__raw_writel(boot_addr, cpu_boot_reg(phys_cpu));

		call_firmware_op(cpu_boot, phys_cpu);
=======
=======
>>>>>>> v3.18
		ret = call_firmware_op(set_cpu_boot_addr, core_id, boot_addr);
		if (ret && ret != -ENOSYS)
			goto fail;
		if (ret == -ENOSYS) {
			void __iomem *boot_reg = cpu_boot_reg(core_id);

			if (IS_ERR(boot_reg)) {
				ret = PTR_ERR(boot_reg);
				goto fail;
			}
			__raw_writel(boot_addr, boot_reg);
		}

		call_firmware_op(cpu_boot, core_id);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		arch_send_wakeup_ipi_mask(cpumask_of(cpu));

		if (pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
=======
=======
>>>>>>> v3.18
fail:
	spin_unlock(&boot_lock);

	return pen_release != -1 ? ret : 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */

static void __init exynos_smp_init_cpus(void)
{
	void __iomem *scu_base = scu_base_addr();
	unsigned int i, ncores;

<<<<<<< HEAD
<<<<<<< HEAD
	if (soc_is_exynos5250())
		ncores = 2;
	else
		ncores = scu_base ? scu_get_core_count(scu_base) : 1;
=======
=======
>>>>>>> v3.18
	if (read_cpuid_part() == ARM_CPU_PART_CORTEX_A9)
		ncores = scu_base ? scu_get_core_count(scu_base) : 1;
	else
		/*
		 * CPU Nodes are passed thru DT and set_cpu_possible
		 * is set by "arm_dt_init_cpu_maps".
		 */
		return;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* sanity check */
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
}

static void __init exynos_smp_prepare_cpus(unsigned int max_cpus)
{
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!(soc_is_exynos5250() || soc_is_exynos5440()))
=======
	exynos_sysram_init();

	if (read_cpuid_part() == ARM_CPU_PART_CORTEX_A9)
>>>>>>> v3.18
=======
	exynos_sysram_init();

	if (read_cpuid_part() == ARM_CPU_PART_CORTEX_A9)
>>>>>>> v3.18
		scu_enable(scu_base_addr());

	/*
	 * Write the address of secondary startup into the
	 * system-wide flags register. The boot monitor waits
	 * until it receives a soft interrupt, and then the
	 * secondary CPU branches to this address.
	 *
	 * Try using firmware operation first and fall back to
	 * boot register if it fails.
	 */
	for (i = 1; i < max_cpus; ++i) {
<<<<<<< HEAD
<<<<<<< HEAD
		unsigned long phys_cpu;
		unsigned long boot_addr;

		phys_cpu = cpu_logical_map(i);
		boot_addr = virt_to_phys(exynos4_secondary_startup);

		if (call_firmware_op(set_cpu_boot_addr, phys_cpu, boot_addr))
			__raw_writel(boot_addr, cpu_boot_reg(phys_cpu));
=======
=======
>>>>>>> v3.18
		unsigned long boot_addr;
		u32 mpidr;
		u32 core_id;
		int ret;

		mpidr = cpu_logical_map(i);
		core_id = MPIDR_AFFINITY_LEVEL(mpidr, 0);
		boot_addr = virt_to_phys(exynos4_secondary_startup);

		ret = call_firmware_op(set_cpu_boot_addr, core_id, boot_addr);
		if (ret && ret != -ENOSYS)
			break;
		if (ret == -ENOSYS) {
			void __iomem *boot_reg = cpu_boot_reg(core_id);

			if (IS_ERR(boot_reg))
				break;
			__raw_writel(boot_addr, boot_reg);
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
}

struct smp_operations exynos_smp_ops __initdata = {
	.smp_init_cpus		= exynos_smp_init_cpus,
	.smp_prepare_cpus	= exynos_smp_prepare_cpus,
	.smp_secondary_init	= exynos_secondary_init,
	.smp_boot_secondary	= exynos_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= exynos_cpu_die,
#endif
};
