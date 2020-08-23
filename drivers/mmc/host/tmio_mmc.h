/*
 * linux/drivers/mmc/host/tmio_mmc.h
 *
 * Copyright (C) 2007 Ian Molton
 * Copyright (C) 2004 Ian Molton
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Driver for the MMC / SD / SDIO cell found in:
 *
 * TC6393XB TC6391XB TC6387XB T7L66XB ASIC3
 */

#ifndef TMIO_MMC_H
#define TMIO_MMC_H

#include <linux/highmem.h>
#include <linux/mmc/tmio.h>
#include <linux/mutex.h>
#include <linux/pagemap.h>
#include <linux/scatterlist.h>
#include <linux/spinlock.h>

/* Definitions for values the CTRL_SDIO_STATUS register can take. */
#define TMIO_SDIO_STAT_IOIRQ	0x0001
#define TMIO_SDIO_STAT_EXPUB52	0x4000
#define TMIO_SDIO_STAT_EXWT	0x8000
#define TMIO_SDIO_MASK_ALL	0xc007

/* Define some IRQ masks */
/* This is the mask used at reset by the chip */
#define TMIO_MASK_ALL           0x837f031d
#define TMIO_MASK_READOP  (TMIO_STAT_RXRDY | TMIO_STAT_DATAEND)
#define TMIO_MASK_WRITEOP (TMIO_STAT_TXRQ | TMIO_STAT_DATAEND)
#define TMIO_MASK_CMD     (TMIO_STAT_CMDRESPEND | TMIO_STAT_CMDTIMEOUT | \
		TMIO_STAT_CARD_REMOVE | TMIO_STAT_CARD_INSERT)
#define TMIO_MASK_IRQ     (TMIO_MASK_READOP | TMIO_MASK_WRITEOP | TMIO_MASK_CMD)

struct tmio_mmc_data;

struct tmio_mmc_host {
	void __iomem *ctl;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long bus_shift;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct mmc_command      *cmd;
	struct mmc_request      *mrq;
	struct mmc_data         *data;
	struct mmc_host         *mmc;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Controller power state */
	bool			power;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Callbacks for clock / power control */
	void (*set_pwr)(struct platform_device *host, int state);
	void (*set_clk_div)(struct platform_device *host, int state);

	/* pio related stuff */
	struct scatterlist      *sg_ptr;
	struct scatterlist      *sg_orig;
	unsigned int            sg_len;
	unsigned int            sg_off;

	struct platform_device *pdev;
	struct tmio_mmc_data *pdata;

	/* DMA support */
	bool			force_pio;
	struct dma_chan		*chan_rx;
	struct dma_chan		*chan_tx;
	struct tasklet_struct	dma_complete;
	struct tasklet_struct	dma_issue;
	struct scatterlist	bounce_sg;
	u8			*bounce_buf;

	/* Track lost interrupts */
	struct delayed_work	delayed_reset_work;
	struct work_struct	done;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Cache IRQ mask */
	u32			sdcard_irq_mask;
	u32			sdio_irq_mask;
=======
=======
>>>>>>> v3.18
	/* Cache */
	u32			sdcard_irq_mask;
	u32			sdio_irq_mask;
	unsigned int		clk_cache;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	spinlock_t		lock;		/* protect host private data */
	unsigned long		last_req_ts;
	struct mutex		ios_lock;	/* protect set_ios() context */
	bool			native_hotplug;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool			sdio_irq_enabled;
>>>>>>> v3.18
=======
	bool			sdio_irq_enabled;
>>>>>>> v3.18
};

int tmio_mmc_host_probe(struct tmio_mmc_host **host,
			struct platform_device *pdev,
			struct tmio_mmc_data *pdata);
void tmio_mmc_host_remove(struct tmio_mmc_host *host);
void tmio_mmc_do_data_irq(struct tmio_mmc_host *host);

void tmio_mmc_enable_mmc_irqs(struct tmio_mmc_host *host, u32 i);
void tmio_mmc_disable_mmc_irqs(struct tmio_mmc_host *host, u32 i);
irqreturn_t tmio_mmc_irq(int irq, void *devid);
irqreturn_t tmio_mmc_sdcard_irq(int irq, void *devid);
irqreturn_t tmio_mmc_card_detect_irq(int irq, void *devid);
irqreturn_t tmio_mmc_sdio_irq(int irq, void *devid);

static inline char *tmio_mmc_kmap_atomic(struct scatterlist *sg,
					 unsigned long *flags)
{
	local_irq_save(*flags);
	return kmap_atomic(sg_page(sg)) + sg->offset;
}

static inline void tmio_mmc_kunmap_atomic(struct scatterlist *sg,
					  unsigned long *flags, void *virt)
{
	kunmap_atomic(virt - sg->offset);
	local_irq_restore(*flags);
}

#if defined(CONFIG_MMC_SDHI) || defined(CONFIG_MMC_SDHI_MODULE)
void tmio_mmc_start_dma(struct tmio_mmc_host *host, struct mmc_data *data);
void tmio_mmc_enable_dma(struct tmio_mmc_host *host, bool enable);
void tmio_mmc_request_dma(struct tmio_mmc_host *host, struct tmio_mmc_data *pdata);
void tmio_mmc_release_dma(struct tmio_mmc_host *host);
void tmio_mmc_abort_dma(struct tmio_mmc_host *host);
#else
static inline void tmio_mmc_start_dma(struct tmio_mmc_host *host,
			       struct mmc_data *data)
{
}

static inline void tmio_mmc_enable_dma(struct tmio_mmc_host *host, bool enable)
{
}

static inline void tmio_mmc_request_dma(struct tmio_mmc_host *host,
				 struct tmio_mmc_data *pdata)
{
	host->chan_tx = NULL;
	host->chan_rx = NULL;
}

static inline void tmio_mmc_release_dma(struct tmio_mmc_host *host)
{
}

static inline void tmio_mmc_abort_dma(struct tmio_mmc_host *host)
{
}
#endif

#ifdef CONFIG_PM
<<<<<<< HEAD
<<<<<<< HEAD
int tmio_mmc_host_suspend(struct device *dev);
int tmio_mmc_host_resume(struct device *dev);
#else
#define tmio_mmc_host_suspend NULL
#define tmio_mmc_host_resume NULL
#endif

int tmio_mmc_host_runtime_suspend(struct device *dev);
int tmio_mmc_host_runtime_resume(struct device *dev);

static inline u16 sd_ctrl_read16(struct tmio_mmc_host *host, int addr)
{
	return readw(host->ctl + (addr << host->bus_shift));
=======
=======
>>>>>>> v3.18
int tmio_mmc_host_runtime_suspend(struct device *dev);
int tmio_mmc_host_runtime_resume(struct device *dev);
#endif

static inline u16 sd_ctrl_read16(struct tmio_mmc_host *host, int addr)
{
	return readw(host->ctl + (addr << host->pdata->bus_shift));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void sd_ctrl_read16_rep(struct tmio_mmc_host *host, int addr,
		u16 *buf, int count)
{
<<<<<<< HEAD
<<<<<<< HEAD
	readsw(host->ctl + (addr << host->bus_shift), buf, count);
=======
	readsw(host->ctl + (addr << host->pdata->bus_shift), buf, count);
>>>>>>> v3.18
=======
	readsw(host->ctl + (addr << host->pdata->bus_shift), buf, count);
>>>>>>> v3.18
}

static inline u32 sd_ctrl_read32(struct tmio_mmc_host *host, int addr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return readw(host->ctl + (addr << host->bus_shift)) |
	       readw(host->ctl + ((addr + 2) << host->bus_shift)) << 16;
=======
	return readw(host->ctl + (addr << host->pdata->bus_shift)) |
	       readw(host->ctl + ((addr + 2) << host->pdata->bus_shift)) << 16;
>>>>>>> v3.18
=======
	return readw(host->ctl + (addr << host->pdata->bus_shift)) |
	       readw(host->ctl + ((addr + 2) << host->pdata->bus_shift)) << 16;
>>>>>>> v3.18
}

static inline void sd_ctrl_write16(struct tmio_mmc_host *host, int addr, u16 val)
{
	/* If there is a hook and it returns non-zero then there
	 * is an error and the write should be skipped
	 */
	if (host->pdata->write16_hook && host->pdata->write16_hook(host, addr))
		return;
<<<<<<< HEAD
<<<<<<< HEAD
	writew(val, host->ctl + (addr << host->bus_shift));
=======
	writew(val, host->ctl + (addr << host->pdata->bus_shift));
>>>>>>> v3.18
=======
	writew(val, host->ctl + (addr << host->pdata->bus_shift));
>>>>>>> v3.18
}

static inline void sd_ctrl_write16_rep(struct tmio_mmc_host *host, int addr,
		u16 *buf, int count)
{
<<<<<<< HEAD
<<<<<<< HEAD
	writesw(host->ctl + (addr << host->bus_shift), buf, count);
=======
	writesw(host->ctl + (addr << host->pdata->bus_shift), buf, count);
>>>>>>> v3.18
=======
	writesw(host->ctl + (addr << host->pdata->bus_shift), buf, count);
>>>>>>> v3.18
}

static inline void sd_ctrl_write32(struct tmio_mmc_host *host, int addr, u32 val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	writew(val, host->ctl + (addr << host->bus_shift));
	writew(val >> 16, host->ctl + ((addr + 2) << host->bus_shift));
=======
	writew(val, host->ctl + (addr << host->pdata->bus_shift));
	writew(val >> 16, host->ctl + ((addr + 2) << host->pdata->bus_shift));
>>>>>>> v3.18
=======
	writew(val, host->ctl + (addr << host->pdata->bus_shift));
	writew(val >> 16, host->ctl + ((addr + 2) << host->pdata->bus_shift));
>>>>>>> v3.18
}


#endif
