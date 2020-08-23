/*
 * Copyright 2009 Sascha Hauer <s.hauer@pengutronix.de>
 *
 * This code is based on code copyrighted by Freescale,
 * Liam Girdwood, Javier Martin and probably others.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef _IMX_PCM_H
#define _IMX_PCM_H

#include <linux/platform_data/dma-imx.h>

/*
 * Do not change this as the FIQ handler depends on this size
 */
#define IMX_SSI_DMABUF_SIZE	(64 * 1024)

static inline void
imx_pcm_dma_params_init_data(struct imx_dma_data *dma_data,
<<<<<<< HEAD
<<<<<<< HEAD
	int dma, bool shared)
{
	dma_data->dma_request = dma;
	dma_data->priority = DMA_PRIO_HIGH;
	if (shared)
		dma_data->peripheral_type = IMX_DMATYPE_SSI_SP;
	else
		dma_data->peripheral_type = IMX_DMATYPE_SSI;
}

int snd_imx_pcm_mmap(struct snd_pcm_substream *substream,
		     struct vm_area_struct *vma);
int imx_pcm_new(struct snd_soc_pcm_runtime *rtd);
void imx_pcm_free(struct snd_pcm *pcm);

#ifdef CONFIG_SND_SOC_IMX_PCM_DMA
int imx_pcm_dma_init(struct platform_device *pdev);
void imx_pcm_dma_exit(struct platform_device *pdev);
=======
=======
>>>>>>> v3.18
	int dma, enum sdma_peripheral_type peripheral_type)
{
	dma_data->dma_request = dma;
	dma_data->priority = DMA_PRIO_HIGH;
	dma_data->peripheral_type = peripheral_type;
}

struct imx_pcm_fiq_params {
	int irq;
	void __iomem *base;

	/* Pointer to original ssi driver to setup tx rx sizes */
	struct snd_dmaengine_dai_dma_data *dma_params_rx;
	struct snd_dmaengine_dai_dma_data *dma_params_tx;
};

#if IS_ENABLED(CONFIG_SND_SOC_IMX_PCM_DMA)
int imx_pcm_dma_init(struct platform_device *pdev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#else
static inline int imx_pcm_dma_init(struct platform_device *pdev)
{
	return -ENODEV;
}
<<<<<<< HEAD
<<<<<<< HEAD

static inline void imx_pcm_dma_exit(struct platform_device *pdev)
{
}
#endif

#ifdef CONFIG_SND_SOC_IMX_PCM_FIQ
int imx_pcm_fiq_init(struct platform_device *pdev);
#else
static inline int imx_pcm_fiq_init(struct platform_device *pdev)
{
	return -ENODEV;
}
=======
=======
>>>>>>> v3.18
#endif

#if IS_ENABLED(CONFIG_SND_SOC_IMX_PCM_FIQ)
int imx_pcm_fiq_init(struct platform_device *pdev,
		struct imx_pcm_fiq_params *params);
void imx_pcm_fiq_exit(struct platform_device *pdev);
#else
static inline int imx_pcm_fiq_init(struct platform_device *pdev,
		struct imx_pcm_fiq_params *params)
{
	return -ENODEV;
}

static inline void imx_pcm_fiq_exit(struct platform_device *pdev)
{
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#endif /* _IMX_PCM_H */
