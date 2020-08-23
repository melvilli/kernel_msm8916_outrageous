/*
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit.com,linux.intel.com})
 * Licensed under the GPL
 */

#include <sys/ptrace.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/ptrace.h>
=======
#include <asm/ptrace.h>
>>>>>>> v3.18
=======
#include <asm/ptrace.h>
>>>>>>> v3.18

int os_arch_prctl(int pid, int code, unsigned long *addr)
{
        return ptrace(PTRACE_ARCH_PRCTL, pid, (unsigned long) addr, code);
}
