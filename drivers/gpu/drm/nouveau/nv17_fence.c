/*
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs <bskeggs@redhat.com>
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <core/object.h>
#include <core/class.h>
=======
#include <nvif/os.h>
#include <nvif/class.h>
>>>>>>> v3.18
=======
#include <nvif/os.h>
#include <nvif/class.h>
>>>>>>> v3.18

#include "nouveau_drm.h"
#include "nouveau_dma.h"
#include "nv10_fence.h"

int
nv17_fence_sync(struct nouveau_fence *fence,
		struct nouveau_channel *prev, struct nouveau_channel *chan)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nv10_fence_priv *priv = chan->drm->fence;
	u32 value;
	int ret;

	if (!mutex_trylock(&prev->cli->mutex))
=======
=======
>>>>>>> v3.18
	struct nouveau_cli *cli = (void *)nvif_client(&prev->device->base);
	struct nv10_fence_priv *priv = chan->drm->fence;
	struct nv10_fence_chan *fctx = chan->fence;
	u32 value;
	int ret;

	if (!mutex_trylock(&cli->mutex))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -EBUSY;

	spin_lock(&priv->lock);
	value = priv->sequence;
	priv->sequence += 2;
	spin_unlock(&priv->lock);

	ret = RING_SPACE(prev, 5);
	if (!ret) {
		BEGIN_NV04(prev, 0, NV11_SUBCHAN_DMA_SEMAPHORE, 4);
<<<<<<< HEAD
<<<<<<< HEAD
		OUT_RING  (prev, NvSema);
=======
		OUT_RING  (prev, fctx->sema.handle);
>>>>>>> v3.18
=======
		OUT_RING  (prev, fctx->sema.handle);
>>>>>>> v3.18
		OUT_RING  (prev, 0);
		OUT_RING  (prev, value + 0);
		OUT_RING  (prev, value + 1);
		FIRE_RING (prev);
	}

	if (!ret && !(ret = RING_SPACE(chan, 5))) {
		BEGIN_NV04(chan, 0, NV11_SUBCHAN_DMA_SEMAPHORE, 4);
<<<<<<< HEAD
<<<<<<< HEAD
		OUT_RING  (chan, NvSema);
=======
		OUT_RING  (chan, fctx->sema.handle);
>>>>>>> v3.18
=======
		OUT_RING  (chan, fctx->sema.handle);
>>>>>>> v3.18
		OUT_RING  (chan, 0);
		OUT_RING  (chan, value + 1);
		OUT_RING  (chan, value + 2);
		FIRE_RING (chan);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&prev->cli->mutex);
=======
	mutex_unlock(&cli->mutex);
>>>>>>> v3.18
=======
	mutex_unlock(&cli->mutex);
>>>>>>> v3.18
	return 0;
}

static int
nv17_fence_context_new(struct nouveau_channel *chan)
{
	struct nv10_fence_priv *priv = chan->drm->fence;
	struct nv10_fence_chan *fctx;
	struct ttm_mem_reg *mem = &priv->bo->bo.mem;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nouveau_object *object;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u32 start = mem->start * PAGE_SIZE;
	u32 limit = start + mem->size - 1;
	int ret = 0;

	fctx = chan->fence = kzalloc(sizeof(*fctx), GFP_KERNEL);
	if (!fctx)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	nouveau_fence_context_new(&fctx->base);
=======
	nouveau_fence_context_new(chan, &fctx->base);
>>>>>>> v3.18
=======
	nouveau_fence_context_new(chan, &fctx->base);
>>>>>>> v3.18
	fctx->base.emit = nv10_fence_emit;
	fctx->base.read = nv10_fence_read;
	fctx->base.sync = nv17_fence_sync;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = nouveau_object_new(nv_object(chan->cli), chan->handle,
				 NvSema, 0x0002,
				 &(struct nv_dma_class) {
					.flags = NV_DMA_TARGET_VRAM |
						 NV_DMA_ACCESS_RDWR,
					.start = start,
					.limit = limit,
				 }, sizeof(struct nv_dma_class),
				 &object);
=======
=======
>>>>>>> v3.18
	ret = nvif_object_init(chan->object, NULL, NvSema, NV_DMA_FROM_MEMORY,
			       &(struct nv_dma_v0) {
					.target = NV_DMA_V0_TARGET_VRAM,
					.access = NV_DMA_V0_ACCESS_RDWR,
					.start = start,
					.limit = limit,
			       }, sizeof(struct nv_dma_v0),
			       &fctx->sema);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (ret)
		nv10_fence_context_del(chan);
	return ret;
}

void
nv17_fence_resume(struct nouveau_drm *drm)
{
	struct nv10_fence_priv *priv = drm->fence;

	nouveau_bo_wr32(priv->bo, 0, priv->sequence);
}

int
nv17_fence_create(struct nouveau_drm *drm)
{
	struct nv10_fence_priv *priv;
	int ret = 0;

	priv = drm->fence = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->base.dtor = nv10_fence_destroy;
	priv->base.resume = nv17_fence_resume;
	priv->base.context_new = nv17_fence_context_new;
	priv->base.context_del = nv10_fence_context_del;
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&priv->lock);

	ret = nouveau_bo_new(drm->dev, 4096, 0x1000, TTM_PL_FLAG_VRAM,
			     0, 0x0000, NULL, &priv->bo);
=======
=======
>>>>>>> v3.18
	priv->base.contexts = 31;
	priv->base.context_base = fence_context_alloc(priv->base.contexts);
	spin_lock_init(&priv->lock);

	ret = nouveau_bo_new(drm->dev, 4096, 0x1000, TTM_PL_FLAG_VRAM,
			     0, 0x0000, NULL, NULL, &priv->bo);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!ret) {
		ret = nouveau_bo_pin(priv->bo, TTM_PL_FLAG_VRAM);
		if (!ret) {
			ret = nouveau_bo_map(priv->bo);
			if (ret)
				nouveau_bo_unpin(priv->bo);
		}
		if (ret)
			nouveau_bo_ref(NULL, &priv->bo);
	}

	if (ret) {
		nv10_fence_destroy(drm);
		return ret;
	}

	nouveau_bo_wr32(priv->bo, 0x000, 0x00000000);
	return ret;
}
