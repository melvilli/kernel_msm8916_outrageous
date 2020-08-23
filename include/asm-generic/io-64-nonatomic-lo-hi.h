#ifndef _ASM_IO_64_NONATOMIC_LO_HI_H_
#define _ASM_IO_64_NONATOMIC_LO_HI_H_

#include <linux/io.h>
#include <asm-generic/int-ll64.h>

<<<<<<< HEAD
#ifndef readq
static inline __u64 readq(const volatile void __iomem *addr)
=======
static inline __u64 lo_hi_readq(const volatile void __iomem *addr)
>>>>>>> v3.18
{
	const volatile u32 __iomem *p = addr;
	u32 low, high;

	low = readl(p);
	high = readl(p + 1);

	return low + ((u64)high << 32);
}
<<<<<<< HEAD
#endif

#ifndef writeq
static inline void writeq(__u64 val, volatile void __iomem *addr)
=======

static inline void lo_hi_writeq(__u64 val, volatile void __iomem *addr)
>>>>>>> v3.18
{
	writel(val, addr);
	writel(val >> 32, addr + 4);
}
<<<<<<< HEAD
=======

#ifndef readq
#define readq lo_hi_readq
#endif

#ifndef writeq
#define writeq lo_hi_writeq
>>>>>>> v3.18
#endif

#endif	/* _ASM_IO_64_NONATOMIC_LO_HI_H_ */
