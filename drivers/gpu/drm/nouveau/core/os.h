#ifndef __NOUVEAU_OS_H__
#define __NOUVEAU_OS_H__

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/pci.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/platform_device.h>
>>>>>>> v3.18
=======
#include <linux/platform_device.h>
>>>>>>> v3.18
#include <linux/printk.h>
#include <linux/bitops.h>
#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/delay.h>
#include <linux/io-mapping.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/vmalloc.h>
#include <linux/acpi.h>
#include <linux/dmi.h>
#include <linux/reboot.h>
#include <linux/interrupt.h>

#include <asm/unaligned.h>

static inline int
ffsll(u64 mask)
{
	int i;
	for (i = 0; i < 64; i++) {
		if (mask & (1ULL << i))
			return i + 1;
	}
	return 0;
}

=======
=======
>>>>>>> v3.18
#include <linux/acpi.h>
#include <linux/vmalloc.h>
#include <linux/dmi.h>
#include <linux/reboot.h>
#include <linux/interrupt.h>
#include <linux/log2.h>
#include <linux/pm_runtime.h>
#include <linux/power_supply.h>
#include <linux/clk.h>

#include <asm/unaligned.h>

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#ifndef ioread32_native
#ifdef __BIG_ENDIAN
#define ioread16_native ioread16be
#define iowrite16_native iowrite16be
#define ioread32_native  ioread32be
#define iowrite32_native iowrite32be
#else /* def __BIG_ENDIAN */
#define ioread16_native ioread16
#define iowrite16_native iowrite16
#define ioread32_native  ioread32
#define iowrite32_native iowrite32
#endif /* def __BIG_ENDIAN else */
#endif /* !ioread32_native */

#endif
