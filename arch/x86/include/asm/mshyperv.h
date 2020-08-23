#ifndef _ASM_X86_MSHYPER_H
#define _ASM_X86_MSHYPER_H

#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/interrupt.h>
>>>>>>> v3.18
#include <asm/hyperv.h>

struct ms_hyperv_info {
	u32 features;
	u32 hints;
};

extern struct ms_hyperv_info ms_hyperv;

void hyperv_callback_vector(void);
<<<<<<< HEAD
void hyperv_vector_handler(struct pt_regs *regs);
void hv_register_vmbus_handler(int irq, irq_handler_t handler);
=======
#ifdef CONFIG_TRACING
#define trace_hyperv_callback_vector hyperv_callback_vector
#endif
void hyperv_vector_handler(struct pt_regs *regs);
void hv_setup_vmbus_irq(void (*handler)(void));
void hv_remove_vmbus_irq(void);
>>>>>>> v3.18

#endif
