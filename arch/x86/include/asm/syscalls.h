/*
 * syscalls.h - Linux syscall interfaces (arch-specific)
 *
 * Copyright (c) 2008 Jaswinder Singh Rajput
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#ifndef _ASM_X86_SYSCALLS_H
#define _ASM_X86_SYSCALLS_H

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/types.h>

/* Common in X86_32 and X86_64 */
/* kernel/ioport.c */
asmlinkage long sys_ioperm(unsigned long, unsigned long, int);
asmlinkage long sys_iopl(unsigned int);

/* kernel/ldt.c */
asmlinkage int sys_modify_ldt(int, void __user *, unsigned long);

/* kernel/signal.c */
<<<<<<< HEAD
long sys_rt_sigreturn(void);
=======
asmlinkage long sys_rt_sigreturn(void);
>>>>>>> v3.18

/* kernel/tls.c */
asmlinkage long sys_set_thread_area(struct user_desc __user *);
asmlinkage long sys_get_thread_area(struct user_desc __user *);

/* X86_32 only */
#ifdef CONFIG_X86_32

/* kernel/signal.c */
<<<<<<< HEAD
unsigned long sys_sigreturn(void);
=======
asmlinkage unsigned long sys_sigreturn(void);
>>>>>>> v3.18

/* kernel/vm86_32.c */
asmlinkage long sys_vm86old(struct vm86_struct __user *);
asmlinkage long sys_vm86(unsigned long, unsigned long);

#else /* CONFIG_X86_32 */

/* X86_64 only */
/* kernel/process_64.c */
<<<<<<< HEAD
long sys_arch_prctl(int, unsigned long);
=======
asmlinkage long sys_arch_prctl(int, unsigned long);
>>>>>>> v3.18

/* kernel/sys_x86_64.c */
asmlinkage long sys_mmap(unsigned long, unsigned long, unsigned long,
			 unsigned long, unsigned long, unsigned long);

#endif /* CONFIG_X86_32 */
#endif /* _ASM_X86_SYSCALLS_H */
