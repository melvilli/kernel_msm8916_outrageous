/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * RajeshwarR: Dec 11, 2007
 *   -- Added support for Inter Processor Interrupts
 *
 * Vineetg: Nov 1st, 2007
 *    -- Initial Write (Borrowed heavily from ARM)
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
=======
>>>>>>> v3.18
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/profile.h>
<<<<<<< HEAD
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>
#include <linux/spinlock_types.h>
=======
#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/irq.h>
#include <linux/atomic.h>
#include <linux/cpumask.h>
>>>>>>> v3.18
#include <linux/reboot.h>
#include <asm/processor.h>
#include <asm/setup.h>
#include <asm/mach_desc.h>

arch_spinlock_t smp_atomic_ops_lock = __ARCH_SPIN_LOCK_UNLOCKED;
arch_spinlock_t smp_bitops_lock = __ARCH_SPIN_LOCK_UNLOCKED;

struct plat_smp_ops  plat_smp_ops;

/* XXX: per cpu ? Only needed once in early seconday boot */
struct task_struct *secondary_idle_tsk;

/* Called from start_kernel */
void __init smp_prepare_boot_cpu(void)
{
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	unsigned int i;

	for (i = 0; i < NR_CPUS; i++)
		set_cpu_possible(i, true);
}

/* called from init ( ) =>  process 1 */
void __init smp_prepare_cpus(unsigned int max_cpus)
{
	int i;

	/*
	 * Initialise the present map, which describes the set of CPUs
	 * actually populated at the present time.
	 */
	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);
}

void __init smp_cpus_done(unsigned int max_cpus)
{

}

/*
 * After power-up, a non Master CPU needs to wait for Master to kick start it
 *
 * The default implementation halts
 *
 * This relies on platform specific support allowing Master to directly set
 * this CPU's PC (to be @first_lines_of_secondary() and kick start it.
 *
 * In lack of such h/w assist, platforms can override this function
 *   - make this function busy-spin on a token, eventually set by Master
 *     (from arc_platform_smp_wakeup_cpu())
 *   - Once token is available, jump to @first_lines_of_secondary
 *     (using inline asm).
 *
 * Alert: can NOT use stack here as it has not been determined/setup for CPU.
 *        If it turns out to be elaborate, it's better to code it in assembly
 *
 */
<<<<<<< HEAD
void __attribute__((weak)) arc_platform_smp_wait_to_boot(int cpu)
=======
void __weak arc_platform_smp_wait_to_boot(int cpu)
>>>>>>> v3.18
{
	/*
	 * As a hack for debugging - since debugger will single-step over the
	 * FLAG insn - wrap the halt itself it in a self loop
	 */
	__asm__ __volatile__(
	"1:		\n"
	"	flag 1	\n"
	"	b 1b	\n");
}

const char *arc_platform_smp_cpuinfo(void)
{
<<<<<<< HEAD
	return plat_smp_ops.info;
=======
	return plat_smp_ops.info ? : "";
>>>>>>> v3.18
}

/*
 * The very first "C" code executed by secondary
 * Called from asm stub in head.S
 * "current"/R25 already setup by low level boot code
 */
<<<<<<< HEAD
void __cpuinit start_kernel_secondary(void)
=======
void start_kernel_secondary(void)
>>>>>>> v3.18
{
	struct mm_struct *mm = &init_mm;
	unsigned int cpu = smp_processor_id();

	/* MMU, Caches, Vector Table, Interrupts etc */
	setup_processor();

	atomic_inc(&mm->mm_users);
	atomic_inc(&mm->mm_count);
	current->active_mm = mm;
<<<<<<< HEAD
=======
	cpumask_set_cpu(cpu, mm_cpumask(mm));
>>>>>>> v3.18

	notify_cpu_starting(cpu);
	set_cpu_online(cpu, true);

	pr_info("## CPU%u LIVE ##: Executing Code...\n", cpu);

	if (machine_desc->init_smp)
<<<<<<< HEAD
		machine_desc->init_smp(smp_processor_id());

	arc_local_timer_setup(cpu);
=======
		machine_desc->init_smp(cpu);

	arc_local_timer_setup();
>>>>>>> v3.18

	local_irq_enable();
	preempt_disable();
	cpu_startup_entry(CPUHP_ONLINE);
}

/*
 * Called from kernel_init( ) -> smp_init( ) - for each CPU
 *
 * At this point, Secondary Processor  is "HALT"ed:
 *  -It booted, but was halted in head.S
 *  -It was configured to halt-on-reset
 *  So need to wake it up.
 *
 * Essential requirements being where to run from (PC) and stack (SP)
*/
<<<<<<< HEAD
int __cpuinit __cpu_up(unsigned int cpu, struct task_struct *idle)
=======
int __cpu_up(unsigned int cpu, struct task_struct *idle)
>>>>>>> v3.18
{
	unsigned long wait_till;

	secondary_idle_tsk = idle;

	pr_info("Idle Task [%d] %p", cpu, idle);
	pr_info("Trying to bring up CPU%u ...\n", cpu);

	if (plat_smp_ops.cpu_kick)
		plat_smp_ops.cpu_kick(cpu,
				(unsigned long)first_lines_of_secondary);

	/* wait for 1 sec after kicking the secondary */
	wait_till = jiffies + HZ;
	while (time_before(jiffies, wait_till)) {
		if (cpu_online(cpu))
			break;
	}

	if (!cpu_online(cpu)) {
		pr_info("Timeout: CPU%u FAILED to comeup !!!\n", cpu);
		return -1;
	}

	secondary_idle_tsk = NULL;

	return 0;
}

/*
 * not supported here
 */
int __init setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

/*****************************************************************************/
/*              Inter Processor Interrupt Handling                           */
/*****************************************************************************/

<<<<<<< HEAD
/*
 * structures for inter-processor calls
 * A Collection of single bit ipi messages
 *
 */

/*
 * TODO_rajesh investigate tlb message types.
 * IPI Timer not needed because each ARC has an individual Interrupting Timer
 */
enum ipi_msg_type {
	IPI_NOP = 0,
	IPI_RESCHEDULE = 1,
	IPI_CALL_FUNC,
	IPI_CALL_FUNC_SINGLE,
	IPI_CPU_STOP
};

struct ipi_data {
	unsigned long bits;
};

static DEFINE_PER_CPU(struct ipi_data, ipi_data);

static void ipi_send_msg(const struct cpumask *callmap, enum ipi_msg_type msg)
{
	unsigned long flags;
	unsigned int cpu;

	local_irq_save(flags);

	for_each_cpu(cpu, callmap) {
		struct ipi_data *ipi = &per_cpu(ipi_data, cpu);
		set_bit(msg, &ipi->bits);
	}

	/* Call the platform specific cross-CPU call function  */
	if (plat_smp_ops.ipi_send)
		plat_smp_ops.ipi_send((void *)callmap);
=======
enum ipi_msg_type {
	IPI_EMPTY = 0,
	IPI_RESCHEDULE = 1,
	IPI_CALL_FUNC,
	IPI_CPU_STOP,
};

/*
 * In arches with IRQ for each msg type (above), receiver can use IRQ-id  to
 * figure out what msg was sent. For those which don't (ARC has dedicated IPI
 * IRQ), the msg-type needs to be conveyed via per-cpu data
 */

static DEFINE_PER_CPU(unsigned long, ipi_data);

static void ipi_send_msg_one(int cpu, enum ipi_msg_type msg)
{
	unsigned long __percpu *ipi_data_ptr = per_cpu_ptr(&ipi_data, cpu);
	unsigned long old, new;
	unsigned long flags;

	pr_debug("%d Sending msg [%d] to %d\n", smp_processor_id(), msg, cpu);

	local_irq_save(flags);

	/*
	 * Atomically write new msg bit (in case others are writing too),
	 * and read back old value
	 */
	do {
		new = old = *ipi_data_ptr;
		new |= 1U << msg;
	} while (cmpxchg(ipi_data_ptr, old, new) != old);

	/*
	 * Call the platform specific IPI kick function, but avoid if possible:
	 * Only do so if there's no pending msg from other concurrent sender(s).
	 * Otherwise, recevier will see this msg as well when it takes the
	 * IPI corresponding to that msg. This is true, even if it is already in
	 * IPI handler, because !@old means it has not yet dequeued the msg(s)
	 * so @new msg can be a free-loader
	 */
	if (plat_smp_ops.ipi_send && !old)
		plat_smp_ops.ipi_send(cpu);
>>>>>>> v3.18

	local_irq_restore(flags);
}

<<<<<<< HEAD
void smp_send_reschedule(int cpu)
{
	ipi_send_msg(cpumask_of(cpu), IPI_RESCHEDULE);
=======
static void ipi_send_msg(const struct cpumask *callmap, enum ipi_msg_type msg)
{
	unsigned int cpu;

	for_each_cpu(cpu, callmap)
		ipi_send_msg_one(cpu, msg);
}

void smp_send_reschedule(int cpu)
{
	ipi_send_msg_one(cpu, IPI_RESCHEDULE);
>>>>>>> v3.18
}

void smp_send_stop(void)
{
	struct cpumask targets;
	cpumask_copy(&targets, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &targets);
	ipi_send_msg(&targets, IPI_CPU_STOP);
}

void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
	ipi_send_msg(cpumask_of(cpu), IPI_CALL_FUNC_SINGLE);
=======
	ipi_send_msg_one(cpu, IPI_CALL_FUNC);
>>>>>>> v3.18
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	ipi_send_msg(mask, IPI_CALL_FUNC);
}

/*
 * ipi_cpu_stop - handle IPI from smp_send_stop()
 */
<<<<<<< HEAD
static void ipi_cpu_stop(unsigned int cpu)
=======
static void ipi_cpu_stop(void)
>>>>>>> v3.18
{
	machine_halt();
}

<<<<<<< HEAD
static inline void __do_IPI(unsigned long *ops, struct ipi_data *ipi, int cpu)
{
	unsigned long msg = 0;

	do {
		msg = find_next_bit(ops, BITS_PER_LONG, msg+1);

		switch (msg) {
		case IPI_RESCHEDULE:
			scheduler_ipi();
			break;

		case IPI_CALL_FUNC:
			generic_smp_call_function_interrupt();
			break;

		case IPI_CALL_FUNC_SINGLE:
			generic_smp_call_function_single_interrupt();
			break;

		case IPI_CPU_STOP:
			ipi_cpu_stop(cpu);
			break;
		}
	} while (msg < BITS_PER_LONG);

=======
static inline void __do_IPI(unsigned long msg)
{
	switch (msg) {
	case IPI_RESCHEDULE:
		scheduler_ipi();
		break;

	case IPI_CALL_FUNC:
		generic_smp_call_function_interrupt();
		break;

	case IPI_CPU_STOP:
		ipi_cpu_stop();
		break;

	default:
		pr_warn("IPI with unexpected msg %ld\n", msg);
	}
>>>>>>> v3.18
}

/*
 * arch-common ISR to handle for inter-processor interrupts
 * Has hooks for platform specific IPI
 */
irqreturn_t do_IPI(int irq, void *dev_id)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();
	struct ipi_data *ipi = &per_cpu(ipi_data, cpu);
	unsigned long ops;

	if (plat_smp_ops.ipi_clear)
		plat_smp_ops.ipi_clear(cpu, irq);

	/*
	 * XXX: is this loop really needed
	 * And do we need to move ipi_clean inside
	 */
	while ((ops = xchg(&ipi->bits, 0)) != 0)
		__do_IPI(&ops, ipi, cpu);
=======
	unsigned long pending;

	pr_debug("IPI [%ld] received on cpu %d\n",
		 *this_cpu_ptr(&ipi_data), smp_processor_id());

	if (plat_smp_ops.ipi_clear)
		plat_smp_ops.ipi_clear(irq);

	/*
	 * "dequeue" the msg corresponding to this IPI (and possibly other
	 * piggybacked msg from elided IPIs: see ipi_send_msg_one() above)
	 */
	pending = xchg(this_cpu_ptr(&ipi_data), 0);

	do {
		unsigned long msg = __ffs(pending);
		__do_IPI(msg);
		pending &= ~(1U << msg);
	} while (pending);
>>>>>>> v3.18

	return IRQ_HANDLED;
}

/*
 * API called by platform code to hookup arch-common ISR to their IPI IRQ
 */
static DEFINE_PER_CPU(int, ipi_dev);
<<<<<<< HEAD
int smp_ipi_irq_setup(int cpu, int irq)
{
	int *dev_id = &per_cpu(ipi_dev, smp_processor_id());
	return request_percpu_irq(irq, do_IPI, "IPI Interrupt", dev_id);
=======

int smp_ipi_irq_setup(int cpu, int irq)
{
	int *dev = per_cpu_ptr(&ipi_dev, cpu);

	arc_request_percpu_irq(irq, cpu, do_IPI, "IPI Interrupt", dev);

	return 0;
>>>>>>> v3.18
}
