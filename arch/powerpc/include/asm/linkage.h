#ifndef _ASM_POWERPC_LINKAGE_H
#define _ASM_POWERPC_LINKAGE_H

#ifdef CONFIG_PPC64
<<<<<<< HEAD
<<<<<<< HEAD
=======
#if !defined(_CALL_ELF) || _CALL_ELF != 2
>>>>>>> v3.18
=======
#if !defined(_CALL_ELF) || _CALL_ELF != 2
>>>>>>> v3.18
#define cond_syscall(x) \
	asm ("\t.weak " #x "\n\t.set " #x ", sys_ni_syscall\n"		\
	     "\t.weak ." #x "\n\t.set ." #x ", .sys_ni_syscall\n")
#define SYSCALL_ALIAS(alias, name)					\
	asm ("\t.globl " #alias "\n\t.set " #alias ", " #name "\n"	\
	     "\t.globl ." #alias "\n\t.set ." #alias ", ." #name)
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif
>>>>>>> v3.18
=======
#endif
>>>>>>> v3.18

#endif	/* _ASM_POWERPC_LINKAGE_H */
