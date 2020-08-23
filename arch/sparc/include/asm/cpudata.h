#ifndef ___ASM_SPARC_CPUDATA_H
#define ___ASM_SPARC_CPUDATA_H
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

#ifndef __ASSEMBLY__

#include <linux/threads.h>
#include <linux/percpu.h>

extern const struct seq_operations cpuinfo_op;

#endif /* !(__ASSEMBLY__) */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#if defined(__sparc__) && defined(__arch64__)
#include <asm/cpudata_64.h>
#else
#include <asm/cpudata_32.h>
#endif
#endif
