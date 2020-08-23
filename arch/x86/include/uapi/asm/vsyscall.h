#ifndef _UAPI_ASM_X86_VSYSCALL_H
#define _UAPI_ASM_X86_VSYSCALL_H

enum vsyscall_num {
	__NR_vgettimeofday,
	__NR_vtime,
	__NR_vgetcpu,
};

<<<<<<< HEAD
<<<<<<< HEAD
#define VSYSCALL_START (-10UL << 20)
#define VSYSCALL_SIZE 1024
#define VSYSCALL_END (-2UL << 20)
#define VSYSCALL_MAPPED_PAGES 1
#define VSYSCALL_ADDR(vsyscall_nr) (VSYSCALL_START+VSYSCALL_SIZE*(vsyscall_nr))

=======
#define VSYSCALL_ADDR (-10UL << 20)
>>>>>>> v3.18
=======
#define VSYSCALL_ADDR (-10UL << 20)
>>>>>>> v3.18

#endif /* _UAPI_ASM_X86_VSYSCALL_H */
