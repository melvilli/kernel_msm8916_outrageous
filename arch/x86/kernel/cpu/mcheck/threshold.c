/*
 * Common corrected MCE threshold handler code:
 */
#include <linux/interrupt.h>
#include <linux/kernel.h>

#include <asm/irq_vectors.h>
#include <asm/apic.h>
#include <asm/idle.h>
#include <asm/mce.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/trace/irq_vectors.h>
>>>>>>> v3.18
=======
#include <asm/trace/irq_vectors.h>
>>>>>>> v3.18

static void default_threshold_interrupt(void)
{
	printk(KERN_ERR "Unexpected threshold interrupt at vector %x\n",
			 THRESHOLD_APIC_VECTOR);
}

void (*mce_threshold_vector)(void) = default_threshold_interrupt;

<<<<<<< HEAD
<<<<<<< HEAD
asmlinkage void smp_threshold_interrupt(void)
{
	irq_enter();
	exit_idle();
	inc_irq_stat(irq_threshold_count);
	mce_threshold_vector();
	irq_exit();
	/* Ack only at the end to avoid potential reentry */
	ack_APIC_irq();
=======
=======
>>>>>>> v3.18
static inline void __smp_threshold_interrupt(void)
{
	inc_irq_stat(irq_threshold_count);
	mce_threshold_vector();
}

asmlinkage __visible void smp_threshold_interrupt(void)
{
	entering_irq();
	__smp_threshold_interrupt();
	exiting_ack_irq();
}

asmlinkage __visible void smp_trace_threshold_interrupt(void)
{
	entering_irq();
	trace_threshold_apic_entry(THRESHOLD_APIC_VECTOR);
	__smp_threshold_interrupt();
	trace_threshold_apic_exit(THRESHOLD_APIC_VECTOR);
	exiting_ack_irq();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
