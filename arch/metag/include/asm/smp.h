#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/cpumask.h>

#define raw_smp_processor_id() (current_thread_info()->cpu)

enum ipi_msg_type {
	IPI_CALL_FUNC,
<<<<<<< HEAD
<<<<<<< HEAD
	IPI_CALL_FUNC_SINGLE,
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	IPI_RESCHEDULE,
};

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi_mask(const struct cpumask *mask);
<<<<<<< HEAD
<<<<<<< HEAD
#define arch_send_call_function_ipi_mask arch_send_call_function_ipi_mask
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

asmlinkage void secondary_start_kernel(void);

extern void secondary_startup(void);

#ifdef CONFIG_HOTPLUG_CPU
extern void __cpu_die(unsigned int cpu);
extern int __cpu_disable(void);
extern void cpu_die(void);
#endif

extern void smp_init_cpus(void);
#endif /* __ASM_SMP_H */
