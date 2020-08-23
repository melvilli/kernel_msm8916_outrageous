#ifndef _ASM_IO_64_NONATOMIC_HI_LO_H_
#define _ASM_IO_64_NONATOMIC_HI_LO_H_

#include <linux/io.h>
#include <asm-generic/int-ll64.h>

<<<<<<< HEAD
#ifndef readq
static inline __u64 readq(const volatile void __iomem *addr)
=======
static inline __u64 hi_lo_readq(const volatile void __iomem *addr)
>>>>>>> v3.18
{
	const volatile u32 __iomem *p = addr;
	u32 low, high;

	high = readl(p + 1);
	low = readl(p);

	return low + ((u64)high << 32);
}
<<<<<<< HEAD
#endif

#ifndef writeq
static inline void writeq(__u64 val, volatile void __iomem *addr)
=======

static inline void hi_lo_writeq(__u64 val, volatile void __iomem *addr)
>>>>>>> v3.18
{
	writel(val >> 32, addr + 4);
	writel(val, addr);
}
<<<<<<< HEAD
=======

#ifndef readq
#define readq hi_lo_readq
#endif

#ifndef writeq
#define writeq hi_lo_writeq
>>>>>>> v3.18
#endif

#endif	/* _ASM_IO_64_NONATOMIC_HI_LO_H_ */
