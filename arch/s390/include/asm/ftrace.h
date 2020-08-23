#ifndef _ASM_S390_FTRACE_H
#define _ASM_S390_FTRACE_H

#ifndef __ASSEMBLY__

extern void _mcount(void);
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern char ftrace_graph_caller_end;
>>>>>>> v3.18
=======
extern char ftrace_graph_caller_end;
>>>>>>> v3.18

struct dyn_arch_ftrace { };

#define MCOUNT_ADDR ((long)_mcount)


static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	return addr;
}

#endif /* __ASSEMBLY__ */

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_64BIT
#define MCOUNT_INSN_SIZE  12
#else
#define MCOUNT_INSN_SIZE  22
#endif
=======
#define MCOUNT_INSN_SIZE  18

#define ARCH_SUPPORTS_FTRACE_OPS 1
>>>>>>> v3.18
=======
#define MCOUNT_INSN_SIZE  18

#define ARCH_SUPPORTS_FTRACE_OPS 1
>>>>>>> v3.18

#endif /* _ASM_S390_FTRACE_H */
