#ifndef _SPARC64_SYSCALLS_H
#define _SPARC64_SYSCALLS_H

struct pt_regs;

<<<<<<< HEAD
extern asmlinkage long sparc_do_fork(unsigned long clone_flags,
				     unsigned long stack_start,
				     struct pt_regs *regs,
				     unsigned long stack_size);
=======
asmlinkage long sparc_do_fork(unsigned long clone_flags,
			      unsigned long stack_start,
			      struct pt_regs *regs,
			      unsigned long stack_size);
>>>>>>> v3.18

#endif /* _SPARC64_SYSCALLS_H */
