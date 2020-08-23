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

	.globl vdso32_start, vdso32_end
	.balign PAGE_SIZE
vdso32_start:
<<<<<<< HEAD
<<<<<<< HEAD
	.incbin "arch/powerpc/kernel/vdso32/vdso32.so"
=======
	.incbin "arch/powerpc/kernel/vdso32/vdso32.so.dbg"
>>>>>>> v3.18
=======
	.incbin "arch/powerpc/kernel/vdso32/vdso32.so.dbg"
>>>>>>> v3.18
	.balign PAGE_SIZE
vdso32_end:

	.previous
