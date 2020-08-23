#ifndef _SPARC_BUG_H
#define _SPARC_BUG_H

#ifdef CONFIG_BUG
#include <linux/compiler.h>

#ifdef CONFIG_DEBUG_BUGVERBOSE
<<<<<<< HEAD
<<<<<<< HEAD
extern void do_BUG(const char *file, int line);
=======
void do_BUG(const char *file, int line);
>>>>>>> v3.18
=======
void do_BUG(const char *file, int line);
>>>>>>> v3.18
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
	__builtin_trap();				\
} while (0)
#else
#define BUG()		__builtin_trap()
#endif

#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

struct pt_regs;
<<<<<<< HEAD
<<<<<<< HEAD
extern void die_if_kernel(char *str, struct pt_regs *regs) __attribute__ ((noreturn));
=======
void __noreturn die_if_kernel(char *str, struct pt_regs *regs);
>>>>>>> v3.18
=======
void __noreturn die_if_kernel(char *str, struct pt_regs *regs);
>>>>>>> v3.18

#endif
