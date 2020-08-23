#ifndef ASMARM_DMA_CONTIGUOUS_H
#define ASMARM_DMA_CONTIGUOUS_H

#ifdef __KERNEL__
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_CMA

#include <linux/types.h>
#include <asm-generic/dma-contiguous.h>

void dma_contiguous_early_fixup(phys_addr_t base, unsigned long size);
void __init dma_contiguous_early_removal_fixup(void);
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_DMA_CMA

#include <linux/types.h>

void dma_contiguous_early_fixup(phys_addr_t base, unsigned long size);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif
#endif

#endif
