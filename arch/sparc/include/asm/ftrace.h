#ifndef _ASM_SPARC64_FTRACE
#define _ASM_SPARC64_FTRACE

#ifdef CONFIG_MCOUNT
#define MCOUNT_ADDR		((long)(_mcount))
#define MCOUNT_INSN_SIZE	4 /* sizeof mcount call */

#ifndef __ASSEMBLY__
<<<<<<< HEAD
extern void _mcount(void);
=======
void _mcount(void);
>>>>>>> v3.18
#endif

#endif

#ifdef CONFIG_DYNAMIC_FTRACE
/* reloction of mcount call site is the same as the address */
static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	return addr;
}

struct dyn_arch_ftrace {
};
#endif /*  CONFIG_DYNAMIC_FTRACE */

<<<<<<< HEAD
=======
unsigned long prepare_ftrace_return(unsigned long parent,
				    unsigned long self_addr,
				    unsigned long frame_pointer);

>>>>>>> v3.18
#endif /* _ASM_SPARC64_FTRACE */
