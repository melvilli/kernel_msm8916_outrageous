<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <linux/linkage.h>
#include <asm/page.h>

	__PAGE_ALIGNED_DATA

	.globl vdso64_start, vdso64_end
	.balign PAGE_SIZE
vdso64_start:
<<<<<<< HEAD
<<<<<<< HEAD
	.incbin "arch/powerpc/kernel/vdso64/vdso64.so"
=======
	.incbin "arch/powerpc/kernel/vdso64/vdso64.so.dbg"
>>>>>>> v3.18
=======
	.incbin "arch/powerpc/kernel/vdso64/vdso64.so.dbg"
>>>>>>> v3.18
	.balign PAGE_SIZE
vdso64_end:

	.previous
