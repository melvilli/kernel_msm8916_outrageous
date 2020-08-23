#ifndef __MACH_ATMEL_MCI_H
#define __MACH_ATMEL_MCI_H

<<<<<<< HEAD
#include <linux/dw_dmac.h>
=======
#include <linux/platform_data/dma-dw.h>
>>>>>>> v3.18

/**
 * struct mci_dma_data - DMA data for MCI interface
 */
struct mci_dma_data {
	struct dw_dma_slave	sdata;
};

/* accessor macros */
#define	slave_data_ptr(s)	(&(s)->sdata)
#define find_slave_dev(s)	((s)->sdata.dma_dev)

#endif /* __MACH_ATMEL_MCI_H */
