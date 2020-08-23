#ifndef _ASM_X86_DMI_H
#define _ASM_X86_DMI_H

#include <linux/compiler.h>
#include <linux/init.h>

#include <asm/io.h>
#include <asm/setup.h>

static __always_inline __init void *dmi_alloc(unsigned len)
{
	return extend_brk(len, sizeof(int));
}

/* Use early IO mappings for DMI because it's initialized early */
<<<<<<< HEAD
<<<<<<< HEAD
#define dmi_ioremap early_ioremap
#define dmi_iounmap early_iounmap
=======
=======
>>>>>>> v3.18
#define dmi_early_remap		early_ioremap
#define dmi_early_unmap		early_iounmap
#define dmi_remap		ioremap
#define dmi_unmap		iounmap
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* _ASM_X86_DMI_H */
