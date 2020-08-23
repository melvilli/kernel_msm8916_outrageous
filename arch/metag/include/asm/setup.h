#ifndef _ASM_METAG_SETUP_H
#define _ASM_METAG_SETUP_H

#include <uapi/asm/setup.h>

<<<<<<< HEAD
=======
extern const struct machine_desc *setup_machine_fdt(void *dt);
>>>>>>> v3.18
void per_cpu_trap_init(unsigned long);
extern void __init dump_machine_table(void);
#endif /* _ASM_METAG_SETUP_H */
